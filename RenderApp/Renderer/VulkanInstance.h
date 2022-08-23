#pragma once
#include "vulkan/vulkan.hpp"
struct VkExtensionProperties;


class VulkanInstance {
public:
	VulkanInstance();
	~VulkanInstance();
	VulkanInstance(const std::vector<VkExtensionProperties>&);

	const std::vector<VkExtensionProperties>& GetSupportedExtensions() const { return m_SupportedExtensions; }
	const std::vector<VkExtensionProperties>& GetRequiredExtensions() const { return m_RequiredExtensions; }
	static std::vector<VkExtensionProperties> GetExtensionPropertiesFromStrings(const std::vector<std::string>&);
	static void EnableValidationLayers(std::vector<const char*> validationLayers) { m_ValidationLayers = validationLayers; }
private:
	const char** getExtensionNames(const std::vector<VkExtensionProperties>&);
	bool HasRequiredExtensions(const std::vector<VkExtensionProperties>&);
	bool HasValidationLayersSupport(const std::vector<const char*>&);
#ifdef _GLFW_
	void SetGLFWRequiredExtensions();
#endif // _GLFW_
	static std::vector<const char*> MakeVector();

private:
	VkInstance m_Instance;
	std::vector<VkExtensionProperties> m_SupportedExtensions;
	std::vector<VkExtensionProperties> m_RequiredExtensions;
	static std::vector<const char*> m_ValidationLayers;
	const char** m_CurrentExtensionNames = nullptr;

};