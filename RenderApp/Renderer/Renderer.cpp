#include "Renderpch.h"
#include "Renderer.h"


#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>

#include "VulkanInstance.h"

void Renderer::Init()
{
	std::vector<const char*> extensionNames = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME };
	if (VulkanInstance::HasEnabledValidationLayers()) {
		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	m_Instance = new VulkanInstance(extensionNames);
	
}

void Renderer::Render()
{
	
}

void Renderer::ShutDown()
{
	delete m_Instance;
}

void Renderer::EnableValidationLayers(const std::vector<const char*>& validationLayers)
{
	VulkanInstance::EnableValidationLayers(validationLayers);
}



