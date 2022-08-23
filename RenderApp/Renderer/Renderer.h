#pragma once
#include "vulkan/vulkan.hpp"

class Window;
class VulkanInstance;
class Renderer
{
public:
	Renderer(Window* window) : m_Window(window) {}
	void Init();
	void Render();
	void ShutDown();
	void EnableValidationLayers(const std::vector<const char*>&);
private:
private:
	Window* m_Window = nullptr;

	VulkanInstance* m_Instance = nullptr;
};

