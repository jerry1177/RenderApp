#pragma once
#include "VDevice.h"
typedef struct VkQueue_T* VkQueue;
namespace VEE {
	class VWindowsSurface;

	class VGraphicsDevice : public VDevice
	{
	public:
		VGraphicsDevice(VInstance*, std::vector<const char*>&, VWindowsSurface*);
		~VGraphicsDevice() {}
		VkQueue GetGraphicsQueue() const { m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { m_PresentQueue; }
	private:
		void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&, VWindowsSurface*);
		bool IsDeviceSuitable(VkPhysicalDevice, VWindowsSurface*);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits, VWindowsSurface*);
		void CreatLogicalDevice(VkPhysicalDevice, VWindowsSurface*);
	private:
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		QueueFamilyIndices m_Indices;
	};
}