#pragma once
struct VInstance;
struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() {
		return graphicsFamily.has_value();
	}
};

class VDevice {
public:
	VDevice(VInstance*);
	uint32_t GetPysicalDeviceCount() { return m_PhysicalDevices.size(); }
private:
	QueueFamilyIndices VDevice::findQueueFamilies(VkPhysicalDevice device);
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
private:
	VkPhysicalDevice m_PickedPhysicalDevice = nullptr;
	std::vector<VkPhysicalDevice> m_PhysicalDevices;
};