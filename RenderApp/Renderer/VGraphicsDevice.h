#pragma once
#include "VDevice.h"
typedef struct VkQueue_T* VkQueue;
namespace VEE {
	class VWindowsSurface;

	class VGraphicsDevice : public VDevice
	{
	public:
		VGraphicsDevice(VInstance*, VWindowsSurface*);
		~VGraphicsDevice() {}
		VkQueue GetGraphicsQueue() const { m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { m_PresentQueue; }
	private:
		void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&) override;
		bool IsDeviceSuitable(VkPhysicalDevice) override;
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits) override;
		void CreatLogicalDevice(VkPhysicalDevice) override;
	private:
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		QueueFamilyIndices m_Indices;
		VWindowsSurface* m_Surface;
	};
}