#pragma once
#include "vulkan/vulkan.h"
struct VkExtensionProperties;

class VulkanInstance {
public:
	VulkanInstance();
	void CreateVKInstance(const std::vector<VkExtensionProperties>&);
	const std::vector<VkExtensionProperties>& GetSupportedExtensions() const { return m_SupportedExtensions; }
	const std::vector<VkExtensionProperties>& GetRequiredExtensions() const { return m_RequiredExtensions; }
private:
	const char** getExtensionNames(const std::vector<VkExtensionProperties>&);
	void ClearCurrentExtensionNames();
	bool HasRequiredExtensions(const std::vector<VkExtensionProperties>&);
private:
	VkInstance m_Instance;
	std::vector<VkExtensionProperties> m_SupportedExtensions;
	std::vector<VkExtensionProperties> m_RequiredExtensions;
	const char** m_CurrentExtensionNames = nullptr;

};