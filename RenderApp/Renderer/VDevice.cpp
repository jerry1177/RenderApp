#include "Renderpch.h"
#include "VDevice.h"
#include "VInstance.h"
#include "vulkan/vulkan.hpp"
std::vector<VkPhysicalDevice> VDevice::m_PhysicalDevices;

VDevice::VDevice(VInstance* instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
	m_PhysicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, m_PhysicalDevices.data());
}

std::vector<VkQueueFamilyProperties> VDevice::GetQueueFamilies(const VkPhysicalDevice& device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	return queueFamilies;
}

