#include "Renderpch.h"
#include "VulkanInstance.h"
#include "vulkan/vulkan.hpp"
#ifdef _GLFW_
	#include "GLFW/glfw3.h"
#endif // _GLFW_ 

VulkanInstance::VulkanInstance()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	m_SupportedExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_SupportedExtensions.data());

#ifdef _GLFW_
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (int i = 0; i < glfwExtensionCount; i++) {
		m_RequiredExtensions.push_back(VkExtensionProperties{});
		strcpy(m_RequiredExtensions[i].extensionName, glfwExtensions[i]);
	}
#endif // _GLFW_

}

void VulkanInstance::CreateVKInstance(const std::vector<VkExtensionProperties>& extensions)
{
	
	// check to make sure estensions have the glfw extensions
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
	if (m_CurrentExtensionNames == nullptr)
		m_CurrentExtensionNames = getExtensionNames(extensions);

	createInfo.ppEnabledExtensionNames = m_CurrentExtensionNames;
	createInfo.enabledLayerCount = 0;
	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
	ASSERT(result == VK_SUCCESS, "failed to create instance!");
	
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

void VulkanInstance::ClearCurrentExtensionNames()
{
	delete m_CurrentExtensionNames;
	m_CurrentExtensionNames = nullptr;
}

bool VulkanInstance::HasRequiredExtensions(const std::vector<VkExtensionProperties>& extensions)
{
	
	int requireExtensionSize = m_RequiredExtensions.size();
	std::vector<bool> hasExtensions(requireExtensionSize, false);

	if (requireExtensionSize > 0) {
		for (int requiredExtensionIndex = 0; requiredExtensionIndex < requireExtensionSize; requiredExtensionIndex++) {

			for (int k = 0; k < extensions.size(); k++) {
				if (strcmp(m_RequiredExtensions[requiredExtensionIndex].extensionName, extensions[k].extensionName) == 0) {
					hasExtensions[requiredExtensionIndex] = true;
					break;
				}
			}
		}
		for (bool hasExtension : hasExtensions) {
			if (!hasExtension) return false;
		}
	}
	return true;
}
