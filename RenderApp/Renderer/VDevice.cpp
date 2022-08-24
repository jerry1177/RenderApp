#include "Renderpch.h"
#include "VDevice.h"
#include "VInstance.h"
#include "vulkan/vulkan.hpp"

VDevice::VDevice(VInstance* instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
	m_PhysicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, m_PhysicalDevices.data());

	
	PickPhysicalDevice();
	ASSERT(m_PickedPhysicalDevice != nullptr, "failed to find a suitable GPU!");
}

void VDevice::PickPhysicalDevice()
{
	int i = 0;
	for (const auto & device : m_PhysicalDevices) {
		if (IsDeviceSuitable(device)) {
			m_PickedPhysicalDevice = device;
			i++;
			//break;
		}
	}

}

bool VDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	return indices.isComplete();
}

QueueFamilyIndices VDevice::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;
	// Logic to find queue family indices to populate struct with
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		if (indices.isComplete()) break;
		i++;
	}
	return indices;
}
