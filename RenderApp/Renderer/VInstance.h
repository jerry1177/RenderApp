#pragma once
#include "vulkan/vulkan.hpp"
struct VkExtensionProperties;

namespace VEE {
	class VInstance {
	public:
		VInstance();
		~VInstance();
		VInstance(const std::vector<const char*>&);

		const std::vector<const char*>& GetSupportedExtensions() const { return m_SupportedExtensions; }
		const std::vector<const char*>& GetRequiredExtensions() const { return m_RequiredExtensions; }
		VkInstance GetHandle() const { return m_Instance; }
		static void EnableValidationLayers(std::vector<const char*> validationLayers) { m_ValidationLayers = validationLayers; }
		static bool HasEnabledValidationLayers() { return m_ValidationLayers.size() > 0; }
	private:
		bool HasRequiredExtensions(const std::vector<const char*>&);
		bool HasValidationLayersSupport(const std::vector<const char*>&);
		void SetSupportedExtensions();
#ifdef _GLFW_
		void SetGLFWRequiredExtensions();
#endif // _GLFW_
		void SetupDebugMessenger();
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		std::vector<const char*> m_SupportedExtensions;
		std::vector<const char*> m_RequiredExtensions;
		static std::vector<const char*> m_ValidationLayers;
		const char** m_CurrentExtensionNames = nullptr;

	};
}