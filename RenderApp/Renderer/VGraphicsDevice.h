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
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { return m_PresentQueue; }
		QueueFamilyIndices GetQueueIndecies() const { return m_Indices; }

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice, VkQueueFlagBits, VWindowsSurface*);
	private:
		void PickPhysicalDevice(const std::vector<VkPhysicalDevice>&, VWindowsSurface*);
		bool IsDeviceSuitable(VkPhysicalDevice, VWindowsSurface*);
		void CreatLogicalDevice(VkPhysicalDevice, VWindowsSurface*);
	private:
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		QueueFamilyIndices m_Indices;
	};
}