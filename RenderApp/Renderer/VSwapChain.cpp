#include "Renderpch.h"
#include "VSwapChain.h"
#include "VWindowsSurface.h"
#include "VDevice.h"
#include "vulkan/vulkan.hpp"
#include <Core/Window.h>

namespace VEE {
	SwapChainSupportDetails* VSwapChain::s_Details = nullptr;

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

	const SwapChainSupportDetails* VSwapChain::QuerySupport(VDevice* device, VWindowsSurface* surface)
	{
		if (s_Details) return s_Details;
		
		s_Details = new SwapChainSupportDetails();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->GetPhysicalDeviceHandle(), surface->GetHandle(), &s_Details->capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->GetPhysicalDeviceHandle(), surface->GetHandle(), &formatCount, nullptr);

		if (formatCount != 0) {
			s_Details->formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device->GetPhysicalDeviceHandle(), surface->GetHandle(), &formatCount, s_Details->formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->GetPhysicalDeviceHandle(), surface->GetHandle(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			s_Details->presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device->GetPhysicalDeviceHandle(), surface->GetHandle(), &presentModeCount, s_Details->presentModes.data());
		}

		return s_Details;
	}

	const VEE::SwapChainSupportDetails* VSwapChain::ReQuerySupport(VDevice* device, VWindowsSurface* surface)
	{
		if (s_Details) {
			delete s_Details;
			s_Details = nullptr;
		}
		QuerySupport(device, surface);
	}

}
