#include "Renderpch.h"
#include "VDevice.h"
#include "VInstance.h"
#include "vulkan/vulkan.hpp"
namespace VEE {
	std::vector<VkPhysicalDevice> VDevice::m_PhysicalDevices;
	std::vector<VDeviceName> VDevice::m_PhysicalDeviceNames;

	VDeviceName::VDeviceName(char* name)
	{
		copy(name);
	}

	VDeviceName::~VDeviceName()
	{

	}

	VDeviceName& VDeviceName::operator=(char* name)
	{
		copy(name);
		return *this;
	}

	void VDeviceName::copy(char* name)
	{
		int i = 0;
		while (name[i] != '\0' && i < m_maxSize) {
			m_deviceName[i] = name[i];
			i++;
			m_Size = i;
		}
	}

	VDevice::VDevice(VInstance* instance, std::vector<const char*> requiredExtensions)
	{
		if (m_PhysicalDevices.size() == 0) {
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
			m_PhysicalDevices.resize(deviceCount);
			vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, m_PhysicalDevices.data());
		}
		
		if (m_PhysicalDeviceNames.size() == 0) {
						
			for (int i = 0; i < m_PhysicalDevices.size(); i++) {
				m_PhysicalDeviceNames.resize(m_PhysicalDevices.size());
				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(m_PhysicalDevices[i], &properties);
				m_PhysicalDeviceNames[i] = properties.deviceName;
			}
		}
		m_RequiredDeviceExtensions = requiredExtensions;

		m_VInstance = instance;
	}

	VDevice::~VDevice()
	{		
		vkDestroyDevice(m_LogicalDevice, nullptr);
	}

	
	bool VDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());;

		std::set<std::string> requiredExtensions(m_RequiredDeviceExtensions.begin(), m_RequiredDeviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	float VDevice::ScoreDevice(VkPhysicalDevice device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		VkPhysicalDeviceMemoryProperties devicememProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &devicememProperties);

		uint32_t GCT_BIT = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		uint32_t GT_BIT = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
		uint32_t CT_BIT = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

		float score = 0;
		float perGiga = 0.000000001f;
		score += deviceProperties.limits.maxMemoryAllocationCount * 0.0015f;
		score += devicememProperties.memoryHeaps->size * perGiga;
		score += deviceProperties.limits.maxStorageBufferRange * perGiga;

		for (const auto& queueFamily : queueFamilies) {
			if ((queueFamily.queueFlags & GCT_BIT) == GCT_BIT) {
				score += 1.5f * queueFamily.queueCount;
			}
			else if ((queueFamily.queueFlags & GT_BIT) == GT_BIT) {
				score += 1.0f * queueFamily.queueCount;
			}
			else if ((queueFamily.queueFlags & CT_BIT) == CT_BIT) {
				score += 1.0f * queueFamily.queueCount;
			}
			else if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) { // dedicated transfer queue
				score += 10.0f * queueFamily.queueCount;
			}

			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				score += 1.0f * queueFamily.queueCount;
			}
			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
				score += 1.0f * queueFamily.queueCount;
			}
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				score += 1.0f * queueFamily.queueCount;
			}
		}
		return score;

	}

	std::vector<VkQueueFamilyProperties> VDevice::GetQueueFamilies(VkPhysicalDevice device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		

		return queueFamilies;
	}

	

}
