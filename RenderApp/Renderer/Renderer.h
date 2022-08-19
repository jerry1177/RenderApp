#pragma once
#include "vulkan/vulkan.hpp"
class Window;
class Renderer
{
public:
	Renderer(Window* window) : m_Window(window) {}
	void Init();
	void Render();
	void ShutDown();
private:
	void createInstance();
private:
	Window* m_Window = nullptr;

	VkInstance m_Instance;
};

