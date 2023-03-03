#pragma once
typedef struct VkImageView_T* VkImageView;
typedef struct VkFramebuffer_T* VkFramebuffer;

namespace VEE {
	class VDevice;
	class VRenderPass;
	class VFrameBuffer {
	public: 
		VFrameBuffer(VDevice* device, VkImageView* attachments, uint32_t attachmentCount, VRenderPass* renderPass, uint32_t width, uint32_t height, uint32_t layers);
		VkFramebuffer GetHandle() { return m_FrameBuffer; }
		~VFrameBuffer();
	private:
		VDevice* m_Device = nullptr;
		VkFramebuffer m_FrameBuffer = nullptr;
	};
}