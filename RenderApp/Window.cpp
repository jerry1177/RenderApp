#include "Renderpch.h"
#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window(uint32_t width, uint32_t height) :m_Width(width), m_Height(height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_Window = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}
