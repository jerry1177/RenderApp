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
#include "Renderpass.h"
#include "GraphicsPipeline.h"
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
		VkFormat format = VSwapChain::ChooseSwapSurfaceFormat(m_Surface->QuerySupport(m_Device)->formats).format;
		
		m_RenderPass = new VRenderPass(m_Device, format);

		m_SwapChain = new VSwapChain(graphicsDevice, m_Surface, m_Window, m_RenderPass);
		m_GraphicsPipeline = new VGraphicsPipeline(m_Device, m_RenderPass, m_SwapChain->GetExtent());

		m_CommandPool = new VCommandPool(m_Device, graphicsDevice->GetQueueIndecies().graphicsFamily.value());

		m_CommandBuffers.resize(m_MaxFramesInFlight);
		for (int i = 0; i < m_CommandBuffers.size(); i++) {
			m_CommandBuffers[i] = std::make_shared<VCommandBuffer>(m_Device, m_CommandPool);
		}
		m_ImageAvailableSemaphores.resize(m_MaxFramesInFlight);
		for (int i = 0; i < m_ImageAvailableSemaphores.size(); i++) {
			m_ImageAvailableSemaphores[i] = std::make_shared<VSemaphore>(m_Device);
		}
		m_RenderFinishedSemaphores.resize(m_MaxFramesInFlight);
		for (int i = 0; i < m_RenderFinishedSemaphores.size(); i++) {
			m_RenderFinishedSemaphores[i] = std::make_shared<VSemaphore>(m_Device);
		}

		m_InFlightFences.resize(m_MaxFramesInFlight);
		for (int i = 0; i < m_InFlightFences.size(); i++) {
			m_InFlightFences[i] = std::make_shared<VFence>(m_Device, VK_FENCE_CREATE_SIGNALED_BIT);
		}
	}

	void VRenderer::Render()
	{
		vkWaitForFences(m_Device->GetLogicalDeviceHandle(), 1, &m_InFlightFences[m_CurrentFrame]->GetHandle(), VK_TRUE, UINT64_MAX);
		//vkResetFences(m_Device->GetLogicalDeviceHandle(), 1, &m_InFlightFences[m_CurrentFrame]->GetHandle());

		uint32_t imageIndex;
		VkResult result = m_SwapChain->AcquireNextImageIndex(m_ImageAvailableSemaphores[m_CurrentFrame], imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
			return;
		}
		ASSERT((result == VK_SUCCESS) || (result == VK_SUBOPTIMAL_KHR), "failed to acquire swap chain image!");

		vkResetFences(m_Device->GetLogicalDeviceHandle(), 1, &m_InFlightFences[m_CurrentFrame]->GetHandle());

		m_CommandBuffers[m_CurrentFrame]->Reset();
		m_CommandBuffers[m_CurrentFrame]->Begin();
		m_RenderPass->Begin(m_CommandBuffers[m_CurrentFrame], m_SwapChain->GetFrameBufferAt(imageIndex), m_SwapChain->GetExtent());
		
		m_GraphicsPipeline->Bind(m_CommandBuffers[m_CurrentFrame]);
		SetViewPort();
		SetScissor();

		vkCmdDraw(m_CommandBuffers[m_CurrentFrame]->GetHandle(), 3, 1, 0, 0);

		m_RenderPass->End(m_CommandBuffers[m_CurrentFrame]);
		m_CommandBuffers[m_CurrentFrame]->End();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame]->GetHandle()};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame]->GetHandleRef();

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame]->GetHandle()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		result = vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]->GetHandle());

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

		result = vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FrameBufferResized) {
			m_FrameBufferResized = false;
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
	}

	void VRenderer::ShutDown()
	{
		vkDeviceWaitIdle(m_Device->GetLogicalDeviceHandle());
		m_ImageAvailableSemaphores.clear();
		m_RenderFinishedSemaphores.clear();
		m_InFlightFences.clear();
		delete m_CommandPool;
		delete m_GraphicsPipeline;
		delete m_RenderPass;
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

	void VRenderer::RecreateSwapChain()
	{
		
		vkDeviceWaitIdle(m_Device->GetLogicalDeviceHandle());
		delete m_SwapChain;

		m_SwapChain = new VSwapChain(m_Device, m_Surface, m_Window, m_RenderPass);

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
		m_CommandBuffers[m_CurrentFrame]->SetViewPort(viewport);
	}

	void VRenderer::SetScissor()
	{
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChain->GetExtent();

		m_CommandBuffers[m_CurrentFrame]->SetScissor(scissor);
	}

}

