#include "Renderpch.h"
#include "VSwapChain.h"
#include "VWindowsSurface.h"
#include "VDevice.h"
#include "vulkan/vulkan.hpp"

namespace VEE {
	SwapChainSupportDetails* VSwapChain::s_Details = nullptr;
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
