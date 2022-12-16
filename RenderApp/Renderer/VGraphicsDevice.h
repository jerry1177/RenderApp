#pragma once
#include "VDevice.h"
typedef struct VkQueue_T* VkQueue;
namespace VEE {
	class VGraphicsDevice : public VDevice
	{
	public:
		VGraphicsDevice(VInstance*);
		~VGraphicsDevice() {}
		VkQueue GetGraphicsQueue() { m_GraphicsQueue; }
	private:
		void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&) override;
		bool IsDeviceSuitable(VkPhysicalDevice) override;
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits) override;
		void CreatLogicalDevice(VkPhysicalDevice) override;
	private:
		VkQueue m_GraphicsQueue;
		QueueFamilyIndices m_Indices;
	};
}