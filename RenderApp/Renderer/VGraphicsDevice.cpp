#include "Renderpch.h"
#include "VGraphicsDevice.h"
#include "VInstance.h"
#include "VWindowsSurface.h"
#include "vulkan/vulkan.h"
namespace VEE {
	VGraphicsDevice::VGraphicsDevice(VInstance* instance, VWindowsSurface* surface) : VDevice(instance), m_Surface(surface)
	{
		PickPhysicalDevice(GetPhisicalDevices());
		ASSERT(m_PhysicalDevice != nullptr, "failed to find a suitable GPU!");

		CreatLogicalDevice(m_PhysicalDevice);

		m_Indices = findQueueFamilies(m_PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
		std::cout << m_Indices.graphicsFamily.value() << std::endl;
		vkGetDeviceQueue(m_LogicalDevice, m_Indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		ASSERT(m_GraphicsQueue != nullptr, "failed to get graphics Queue!");
		vkGetDeviceQueue(m_LogicalDevice, m_Indices.presentFamily.value(), 0, &m_PresentQueue);
		ASSERT(m_PresentQueue != nullptr, "failed to get present Queue!");

	}

	void VGraphicsDevice::PickPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) // just pick first suitable device for now
	{
		for (const auto& device : devices) {
			if (IsDeviceSuitable(device)) {
				m_PhysicalDevice = device;
				break;
			}
		}
	}

	bool VGraphicsDevice::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT);

		return indices.graphicsFamily.has_value() && indices.presentFamily.has_value();
	}

	QueueFamilyIndices VGraphicsDevice::findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits queueFlags)
	{
		QueueFamilyIndices indices;
		// Logic to find queue family indices to populate struct with

		std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilies(device);

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			if (queueFamily.queueFlags & queueFlags & VK_QUEUE_TRANSFER_BIT) {
				indices.transferFamily = i;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface->GetHandle(), &presentSupport);
			if (presentSupport) {
				indices.presentFamily = i;
			}
			i++;
		}
		return indices;
	}

	void VGraphicsDevice::CreatLogicalDevice(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = 0;


		if (m_VInstance->HasEnabledValidationLayers()) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_VInstance->GetValidationLayers().size());
			createInfo.ppEnabledLayerNames = m_VInstance->GetValidationLayers().data();
		}
		vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);
		ASSERT(m_LogicalDevice != nullptr, "failed to create logical device!");
		
	}
}
