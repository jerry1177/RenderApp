#include "Renderpch.h"
#include "VulkanInstance.h"
#include "vulkan/vulkan.hpp"
#ifdef _GLFW_
	#include "GLFW/glfw3.h"
#endif // _GLFW_ 

std::vector<const char*> VulkanInstance::m_ValidationLayers;
VulkanInstance::VulkanInstance()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	m_SupportedExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_SupportedExtensions.data());

#ifdef _GLFW_
	SetGLFWRequiredExtensions();
#endif // _GLFW_

}

VulkanInstance::VulkanInstance(const std::vector<VkExtensionProperties>& extensions)
{
#ifdef _GLFW_
	SetGLFWRequiredExtensions();
#endif // _GLFW_
	
	ASSERT(HasValidationLayersSupport(m_ValidationLayers), "validation layers requested, but not available!");
	ASSERT(HasRequiredExtensions(extensions), "Does not have required Extensions!");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;


	createInfo.enabledExtensionCount = extensions.size();
	m_CurrentExtensionNames = getExtensionNames(extensions);

	createInfo.ppEnabledExtensionNames = m_CurrentExtensionNames;
	createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
	createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
	ASSERT(result == VK_SUCCESS, "failed to create instance!");
}


VulkanInstance::~VulkanInstance()
{
	delete m_CurrentExtensionNames;
	vkDestroyInstance(m_Instance, nullptr);
}

std::vector<VkExtensionProperties> VulkanInstance::GetExtensionPropertiesFromStrings(const std::vector<std::string>& extensionNames)
{
	std::vector<VkExtensionProperties> extensionProperties(extensionNames.size(), VkExtensionProperties());
	for (int i = 0; i < extensionProperties.size(); i++) {
		strcpy(extensionProperties[i].extensionName, extensionNames[i].c_str());
	}
	return extensionProperties;
}

/// <summary>
/// This function gets you an array of char pointers to the extension names of the VKExtensionProperties vector you pass in. 
/// Make sure to deallocate the heap allocated array when you are done using it!
/// </summary>
/// <param name="extensions">A list of VkExtensionProperties that you want to get the extension names for.</param>
/// <returns>Heap allocated const Pointer to char* extension names</returns>
const char** VulkanInstance::getExtensionNames(const std::vector<VkExtensionProperties>& extensions)
{
	const char** extensionNames = new const char* [extensions.size()];
		for (int i = 0; i < extensions.size(); i++) {
			extensionNames[i] = extensions[i].extensionName;
		}
	return extensionNames;
}


bool VulkanInstance::HasRequiredExtensions(const std::vector<VkExtensionProperties>& extensions)
{
	
	int requireExtensionSize = m_RequiredExtensions.size();

	if (requireExtensionSize > 0) {
		bool hasExtension = false;
		for (int requiredExtensionIndex = 0; requiredExtensionIndex < requireExtensionSize; requiredExtensionIndex++) {

			for (int k = 0; k < extensions.size(); k++) {
				if (strcmp(m_RequiredExtensions[requiredExtensionIndex].extensionName, extensions[k].extensionName) == 0) {
					hasExtension = true;
					break;
				}
			}
		}
		
		if (!hasExtension) return false;
		
	}
	return true;
}

bool VulkanInstance::HasValidationLayersSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) return false;
	}
	return true;
}

#ifdef _GLFW_
void VulkanInstance::SetGLFWRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (int i = 0; i < glfwExtensionCount; i++) {
		m_RequiredExtensions.push_back(VkExtensionProperties{});
		strcpy(m_RequiredExtensions[i].extensionName, glfwExtensions[i]);
	}
}

std::vector<const char*> VulkanInstance::MakeVector()
{
	std::vector<const char*> myVec;
	return myVec;
}

#endif // _GLFW_