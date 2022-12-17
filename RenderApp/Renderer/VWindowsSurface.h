#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace VEE {
	class VInstance;
	class Window;
	class VWindowsSurface {
	public:
		VWindowsSurface(VInstance*, Window*);
		~VWindowsSurface();
		VkSurfaceKHR GetHandle() const { return m_Surface; }
	private:
		VkSurfaceKHR m_Surface;
		VInstance* m_Instance;
	};
}