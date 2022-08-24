#include "Renderpch.h"
#include "VInstance.h"
#include "vulkan/vulkan.hpp"
#ifdef _GLFW_
	#include "GLFW/glfw3.h"
#endif // _GLFW_ 

std::vector<const char*> VInstance::m_ValidationLayers;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
VInstance::VInstance()
{
	SetSupportedExtensions();

#ifdef _GLFW_
	SetGLFWRequiredExtensions();
#endif // _GLFW_

}

VInstance::VInstance(const std::vector<const char*>& extensions)
{
#ifdef _GLFW_
	SetGLFWRequiredExtensions();
#endif // _GLFW_
	
	ASSERT(HasValidationLayersSupport(m_ValidationLayers), "validation layers requested, but not available!");
	ASSERT(HasRequiredExtensions(extensions), "Does not have required Extensions!");
	SetSupportedExtensions();
	
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

	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
	createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	PopulateDebugMessengerCreateInfo(debugCreateInfo);
	if (HasEnabledValidationLayers()) {
		
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
	ASSERT(result == VK_SUCCESS, "failed to create instance!");
	SetupDebugMessenger();
}


VInstance::~VInstance()
{
	if (HasEnabledValidationLayers())
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	delete m_CurrentExtensionNames;
	vkDestroyInstance(m_Instance, nullptr);
}


bool VInstance::HasRequiredExtensions(const std::vector<const char*>& extensions)
{
	
	int requireExtensionSize = m_RequiredExtensions.size();

	bool hasExtension;
	if (requireExtensionSize > 0) {
		hasExtension = false;
		for (int requiredExtensionIndex = 0; requiredExtensionIndex < requireExtensionSize; requiredExtensionIndex++) {

			for (int k = 0; k < extensions.size(); k++) {
				if (strcmp(m_RequiredExtensions[requiredExtensionIndex], extensions[k]) == 0) {
					hasExtension = true;
					break;
				}
			}
		}
		
		if (!hasExtension) return false;
		
	}
	return true;
}

bool VInstance::HasValidationLayersSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	bool layerFound;
	for (const char* layerName : validationLayers) {
		layerFound = false;

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

void VInstance::SetSupportedExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	m_SupportedExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (const auto& extension : extensions) {
		m_SupportedExtensions.push_back(extension.extensionName);
	}
}

void VInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
void VInstance::SetupDebugMessenger()
{
	if (!VInstance::HasEnabledValidationLayers()) return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);
	ASSERT(CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) == VK_SUCCESS, "failed to set up debug messenger!");
}
VkResult VInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
void VInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

#ifdef _GLFW_
void VInstance::SetGLFWRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (int i = 0; i < glfwExtensionCount; i++) {
		m_RequiredExtensions.push_back(glfwExtensions[i]);
	}
}
#endif // _GLFW_