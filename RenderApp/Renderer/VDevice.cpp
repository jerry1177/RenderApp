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

	VDevice::VDevice(VInstance* instance)
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

		m_VInstance = instance;
	}

	VDevice::~VDevice()
	{		
		vkDestroyDevice(m_LogicalDevice, nullptr);
	}

	
	std::vector<VkQueueFamilyProperties> VDevice::GetQueueFamilies(const VkPhysicalDevice& device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		return queueFamilies;
	}

	

}
