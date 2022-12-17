#include "Renderpch.h"
#include "VWindowsSurface.h"
#include "VInstance.h"
#include "Core/Window.h"
namespace VEE {

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

}