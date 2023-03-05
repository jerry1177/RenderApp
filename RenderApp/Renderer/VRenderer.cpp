#include "Renderpch.h"
#include "VRenderer.h"


#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include "VGraphicsDevice.h"
#include "VInstance.h"
#include "VWindowsSurface.h"
#include "VGraphicsDevice.h"
#include "VSwapChain.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "Semaphore.h"
#include "Fence.h"

namespace VEE {
	void VRenderer::Init()
	{
		std::vector<const char*> extensionNames = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME };
		if (VInstance::HasEnabledValidationLayers()) {
			extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		m_Instance = new VInstance(extensionNames);
		m_Surface = new VWindowsSurface(m_Instance, m_Window);
		std::vector<const char*> deviceExtensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		m_Device = new VGraphicsDevice(m_Instance, deviceExtensionNames, m_Surface);

		VGraphicsDevice* graphicsDevice = (VGraphicsDevice*)m_Device;
		m_SwapChain = new VSwapChain(graphicsDevice, m_Surface, m_Window);

		m_CommandPool = new VCommandPool(m_Device, graphicsDevice->GetQueueIndecies().graphicsFamily.value());
		m_CommandBuffer = new VCommandBuffer(m_Device, m_CommandPool);
		m_ImageAvailableSemaphore = new VSemaphore(m_Device);
		m_RenderFinishedSemaphore = new VSemaphore(m_Device);
		m_InFlightFence = new VFence(m_Device, VK_FENCE_CREATE_SIGNALED_BIT);
	}

	void VRenderer::Render()
	{
		vkWaitForFences(m_Device->GetLogicalDeviceHandle(), 1, &m_InFlightFence->GetHandle(), VK_TRUE, UINT64_MAX);
		vkResetFences(m_Device->GetLogicalDeviceHandle(), 1, &m_InFlightFence->GetHandle());

		uint32_t imageIndex = m_SwapChain->AcquireNextImageIndex(m_ImageAvailableSemaphore);

		m_CommandBuffer->Reset();
		m_CommandBuffer->Begin();
		m_SwapChain->BeginRenderPass(m_CommandBuffer, imageIndex);
		m_SwapChain->BindGraphicsPipeline(m_CommandBuffer);
		SetViewPort();
		SetScissor();

		vkCmdDraw(m_CommandBuffer->GetHandle(), 3, 1, 0, 0);

		m_SwapChain->EndRenderPass(m_CommandBuffer);
		m_CommandBuffer->End();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore->GetHandle() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer->GetHandleRef();

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore->GetHandle() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkResult result = vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFence->GetHandle());

		ASSERT(result == VK_SUCCESS, "failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { m_SwapChain->GetHandle() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);

	}

	void VRenderer::ShutDown()
	{
		delete m_ImageAvailableSemaphore;
		delete m_RenderFinishedSemaphore;
		delete m_InFlightFence;
		delete m_CommandPool;
		delete m_SwapChain;
		delete m_Device;
		delete m_Surface;
		delete m_Instance;
	}

	void VRenderer::EnableValidationLayers(const std::vector<const char*>& validationLayers)
	{
		VInstance::EnableValidationLayers(validationLayers);
	}

	const std::vector<VDeviceName>& VRenderer::GetDeviceNames()
	{
		return m_Device->GetDeviceNames();
	}

	void VRenderer::RecordCommandBuffer()
	{

	}

	void VRenderer::SetViewPort()
	{
		VkExtent2D extent = m_SwapChain->GetExtent();
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		m_CommandBuffer->SetViewPort(viewport);
	}

	void VRenderer::SetScissor()
	{
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChain->GetExtent();

		m_CommandBuffer->SetScissor(scissor);
	}

}

