#include "Renderpch.h"
#include "VRenderer.h"


#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>

#include "VInstance.h"
#include "VDevice.h"
void VRenderer::Init()
{
	std::vector<const char*> extensionNames = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME };
	if (VInstance::HasEnabledValidationLayers()) {
		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	m_Instance = new VInstance(extensionNames);
	m_Device = new VDevice(m_Instance);
	std::cout << "Device count is: " << m_Device->GetDeviceCount() << std::endl;
	
}

void VRenderer::Render()
{
	
}

void VRenderer::ShutDown()
{
	delete m_Instance;
}

void VRenderer::EnableValidationLayers(const std::vector<const char*>& validationLayers)
{
	VInstance::EnableValidationLayers(validationLayers);
}



