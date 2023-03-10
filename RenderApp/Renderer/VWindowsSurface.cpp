#include "Renderpch.h"
#include "VWindowsSurface.h"
#include "VInstance.h"
#include "Core/Window.h"
#include "VDevice.h"
//#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "vulkan/vulkan.hpp"

namespace VEE {

	SurfaceSupportDetails* VWindowsSurface::s_SupportDetails = nullptr;

	VWindowsSurface::VWindowsSurface(VInstance* instance, Window* window) : m_Instance(instance)
	{
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(window->GetGLFWWindow());
		createInfo.hinstance = GetModuleHandle(nullptr);

		VkResult result = vkCreateWin32SurfaceKHR(m_Instance->GetHandle(), &createInfo, nullptr, &m_Surface);
		ASSERT(result == VK_SUCCESS, "failed to create logical device!");
	}

	VWindowsSurface::~VWindowsSurface()
	{
		vkDestroySurfaceKHR(m_Instance->GetHandle(), m_Surface, nullptr);
	}

	const SurfaceSupportDetails* VWindowsSurface::QuerySupport(VDevice* device)
	{
		return QuerySupport(device->GetPhysicalDeviceHandle());
	}

	const SurfaceSupportDetails* VWindowsSurface::QuerySupport(VkPhysicalDevice device)
	{
		if (s_SupportDetails) { delete s_SupportDetails; }

		s_SupportDetails = new SurfaceSupportDetails();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &s_SupportDetails->capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

		if (formatCount != 0) {
			s_SupportDetails->formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, s_SupportDetails->formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			s_SupportDetails->presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, s_SupportDetails->presentModes.data());
		}

		return s_SupportDetails;
	}

	const SurfaceSupportDetails* VWindowsSurface::ReQuerySupport(VDevice* device, VWindowsSurface* surface)
	{
		if (s_SupportDetails) {
			delete s_SupportDetails;
			s_SupportDetails = nullptr;
		}
		return QuerySupport(device);
	}

}