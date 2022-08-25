#pragma once
class VInstance;
struct VkPhysicalDevice_T;
struct VkDevice_T;
typedef struct VkQueueFamilyProperties;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
typedef VkDevice_T* VkDevice;
typedef enum VkQueueFlagBits;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> computeFamily;
	std::optional<uint32_t> transferFamily;
	bool hasAllFamilies() {
		return graphicsFamily.has_value() && computeFamily.has_value() && transferFamily.has_value();
	}
};

class VDevice {
public:
	VDevice(VInstance*);
	virtual ~VDevice() {}
	uint32_t GetPysicalDeviceCount() { return m_PhysicalDevices.size(); }
	const VkPhysicalDevice& GetPhysicalDeviceHandle() const { return m_PhysicalDevice; }
	const VkDevice& GetLogicalDeviceHandle() const { return m_LogicalDevice; }
	const std::vector<VkPhysicalDevice>& GetPhisicalDevices() const { return m_PhysicalDevices; }
	
protected:
	std::vector< VkQueueFamilyProperties> GetQueueFamilies(const VkPhysicalDevice&);
	virtual void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&) = 0;
	virtual bool IsDeviceSuitable(VkPhysicalDevice) = 0;
	virtual QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits) = 0;
	virtual void CreatLogicalDevice(VkPhysicalDevice) = 0;
protected:
	VkDevice m_LogicalDevice = nullptr;
	VkPhysicalDevice m_PhysicalDevice = nullptr;
private:
	static std::vector<VkPhysicalDevice> m_PhysicalDevices;
};