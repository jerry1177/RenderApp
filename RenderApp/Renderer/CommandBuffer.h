#pragma once

typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkViewport;
struct VkRect2D;

namespace VEE {
	class VDevice;
	class VCommandPool;

	class VCommandBuffer {
	public:
		VCommandBuffer(VDevice*, VCommandPool*);
		VkCommandBuffer GetHandle() const { return m_CommandBuffer; }
		void VCommandBuffer::SetViewPort(VkViewport viewPort);
		void VCommandBuffer::SetScissor(VkRect2D scissor);

		void Begin();
		void End();
		~VCommandBuffer();

	private:
		VDevice* m_Device;
		VkCommandBuffer m_CommandBuffer;
	};
}