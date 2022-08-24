#pragma once
struct VInstance;
struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
class VDevice {
public:
	VDevice(VInstance*);
	uint32_t GetDeviceCount() { return m_Devices.size(); }
private:
	VkPhysicalDevice physicalDevice = nullptr;
	std::vector<VkPhysicalDevice> m_Devices;
};