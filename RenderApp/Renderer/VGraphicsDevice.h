#pragma once
#include "VDevice.h"

class VGraphicsDevice : public VDevice
{
public:
	VGraphicsDevice(VInstance*);
	~VGraphicsDevice(){}

private:
	void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&) override;
	bool IsDeviceSuitable(VkPhysicalDevice) override;
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits) override;
	void CreatLogicalDevice(VkPhysicalDevice) override;
};