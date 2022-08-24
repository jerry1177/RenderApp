#include "Renderpch.h"
#include "VDevice.h"
#include "VInstance.h"
#include "vulkan/vulkan.hpp"

VDevice::VDevice(VInstance* instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
	m_Devices.resize(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, m_Devices.data());
}
