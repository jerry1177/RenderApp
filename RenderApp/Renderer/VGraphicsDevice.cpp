#include "Renderpch.h"
#include "VGraphicsDevice.h"
#include "vulkan/vulkan.h"
namespace VEE {
	VGraphicsDevice::VGraphicsDevice(VInstance* instance) : VDevice(instance)
	{
		PickPhysicalDevice(GetPhisicalDevices());
		ASSERT(m_PhysicalDevice != nullptr, "failed to find a suitable GPU!");
		//CreatLogicalDevice(m_PhysicalDevice);
	}

	void VGraphicsDevice::PickPhysicalDevice(const std::vector<VkPhysicalDevice>& devices)
	{
		int i = 0;
		for (const auto& device : devices) {
			if (IsDeviceSuitable(device)) {
				m_PhysicalDevice = device;
				i++;
				//break;
			}
		}
	}

	bool VGraphicsDevice::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT);

		return indices.graphicsFamily.has_value();
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
			i++;
		}
		return indices;
	}

	void VGraphicsDevice::CreatLogicalDevice(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	}
}
