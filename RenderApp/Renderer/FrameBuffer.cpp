#include "Renderpch.h"
#include "FrameBuffer.h"
#include "VDevice.h"
#include "Renderpass.h"
#include "vulkan/vulkan.hpp"

namespace VEE {

	VFrameBuffer::VFrameBuffer(VDevice* device , VkImageView* attachments, uint32_t attachmentCount, VRenderPass* renderPass, uint32_t width, uint32_t height, uint32_t layers) : m_Device(device)
	{
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->GetHandle();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = layers;
		vkCreateFramebuffer(m_Device->GetLogicalDeviceHandle(), &framebufferInfo, nullptr, &m_FrameBuffer);
		ASSERT(m_FrameBuffer != nullptr, "failed to create a Framebuffer!");
	}

	VFrameBuffer::~VFrameBuffer()
	{
		vkDestroyFramebuffer(m_Device->GetLogicalDeviceHandle(), m_FrameBuffer, nullptr);
	}
}

