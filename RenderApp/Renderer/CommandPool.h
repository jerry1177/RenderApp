#pragma once

typedef struct VkCommandPool_T* VkCommandPool;

namespace VEE {
	class VDevice;
	class VCommandPool
	{
	public:
		VCommandPool(VDevice*, uint32_t);
		VkCommandPool GetHandle() { return m_CommandPool; }
		~VCommandPool();

	private:
		VDevice* m_Device;
		VkCommandPool m_CommandPool;
	};

}