#pragma once

typedef struct VkCommandBuffer_T* VkCommandBuffer;

namespace VEE {
	class VDevice;
	class VCommandPool;

	class VCommandBuffer {
	public:
		VCommandBuffer(VDevice*, VCommandPool*);
		~VCommandBuffer();

	private:
		VDevice* m_Device;
		VkCommandBuffer m_CommandBuffer;
	};
}