#include "Renderpch.h"
#include "VSwapChain.h"
#include "VWindowsSurface.h"
#include "VDevice.h"
#include "VGraphicsDevice.h"
#include "VertShader.h"
#include "FragShader.h"
#include "Renderpass.h"
#include "FrameBuffer.h"
#include "CommandBuffer.h"
#include "Semaphore.h"
#include "vulkan/vulkan.h"
#include <Core/Window.h>
#include <future>


namespace VEE {

	//SurfaceSupportDetails* VSwapChain::s_Details = nullptr;

	VSwapChain::VSwapChain(VGraphicsDevice* device, VWindowsSurface* surface, Window* window, VRenderPass* renderPass) : m_Device(device)
	{
		const SurfaceSupportDetails* swapChainSupport = surface->QuerySupport(device);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport->formats);
		m_ImageFormat = surfaceFormat.format;
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport->presentModes);
		m_Extent = ChooseSwapExtent(swapChainSupport->capabilities, window);

		m_ImageCount = swapChainSupport->capabilities.minImageCount + 1;
		if (swapChainSupport->capabilities.maxImageCount > 0 && m_ImageCount > swapChainSupport->capabilities.maxImageCount) {
			m_ImageCount = swapChainSupport->capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface->GetHandle();
		createInfo.minImageCount = m_ImageCount;
		createInfo.imageFormat = m_ImageFormat;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = device->findQueueFamilies(device->GetPhysicalDeviceHandle(), (VkQueueFlagBits)(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT), surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport->capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		vkCreateSwapchainKHR(device->GetLogicalDeviceHandle(), &createInfo, nullptr, &m_SwapChain);
		ASSERT(m_SwapChain != nullptr, "failed to create SwapChain!");

		// get Images
		vkGetSwapchainImagesKHR(device->GetLogicalDeviceHandle(), m_SwapChain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(device->GetLogicalDeviceHandle(), m_SwapChain, &m_ImageCount, m_Images.data());
		// create Image views
		m_ImageViews.resize(m_Images.size());
		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_ImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			vkCreateImageView(m_Device->GetLogicalDeviceHandle(), &createInfo, nullptr, &m_ImageViews[i]);
			ASSERT(m_ImageViews[i] != nullptr, "failed to create Imageview!")
		}

		m_FrameBuffers.resize(m_ImageViews.size());

		for (size_t i = 0; i < m_FrameBuffers.size(); i++) {
			VkImageView attachments[] = { m_ImageViews[i] };
			m_FrameBuffers[i] = new VFrameBuffer(m_Device, attachments, 1, renderPass, m_Extent.width, m_Extent.height, 1);
		}

	}

	VSwapChain::~VSwapChain()
	{
		//delete m_ImageAvailableSemaphore;

		for (auto framebuffer : m_FrameBuffers) {
			delete framebuffer;
		}

		for (auto imageView : m_ImageViews) {
			vkDestroyImageView(m_Device->GetLogicalDeviceHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device->GetLogicalDeviceHandle(), m_SwapChain, nullptr);
	}

	VkResult VSwapChain::AcquireNextImageIndex(std::shared_ptr<VSemaphore>& semaphore, uint32_t& outImageIndex)
	{
		VkResult result = vkAcquireNextImageKHR(m_Device->GetLogicalDeviceHandle(), m_SwapChain, UINT64_MAX, semaphore->GetHandle(), VK_NULL_HANDLE, &outImageIndex);
		
		return result;
	}

	VkSurfaceFormatKHR VSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		// TODO: rank available formats and 
		return availableFormats[0];
	}

	VkPresentModeKHR VSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window)
	{
		#undef max // work around for max macro definition
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			WindowFrameBufferSize windowSize = window->GetFrameBufferSize();

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(windowSize.Width),
				static_cast<uint32_t>(windowSize.Height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

}
