#include "Renderpch.h"
#include "VSwapChain.h"
#include "VWindowsSurface.h"
#include "VDevice.h"
#include "VGraphicsDevice.h"
#include "vulkan/vulkan.hpp"
#include <Core/Window.h>


namespace VEE {
	SwapChainSupportDetails* VSwapChain::s_Details = nullptr;

	VSwapChain::VSwapChain(VGraphicsDevice* device, VWindowsSurface* surface, Window* window) : m_Device(device)
	{
		const SwapChainSupportDetails* swapChainSupport = QuerySupport(device->GetPhysicalDeviceHandle(), surface);

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

	}

	VSwapChain::~VSwapChain()
	{
		for (auto imageView : m_ImageViews) {
			vkDestroyImageView(m_Device->GetLogicalDeviceHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device->GetLogicalDeviceHandle(), m_SwapChain, nullptr);
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

	const SwapChainSupportDetails* VSwapChain::QuerySupport(VkPhysicalDevice device, VWindowsSurface* surface)
	{
		if (s_Details) return s_Details;
		
		s_Details = new SwapChainSupportDetails();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface->GetHandle(), &s_Details->capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetHandle(), &formatCount, nullptr);

		if (formatCount != 0) {
			s_Details->formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetHandle(), &formatCount, s_Details->formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetHandle(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			s_Details->presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetHandle(), &presentModeCount, s_Details->presentModes.data());
		}

		return s_Details;
	}

	const VEE::SwapChainSupportDetails* VSwapChain::ReQuerySupport(VDevice* device, VWindowsSurface* surface)
	{
		if (s_Details) {
			delete s_Details;
			s_Details = nullptr;
		}
		QuerySupport(device->GetPhysicalDeviceHandle(), surface);
	}

}
