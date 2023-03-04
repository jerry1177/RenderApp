
#include "Renderpch.h"
#include "CommandBuffer.h"
#include "VDevice.h"
#include "CommandPool.h"
#include "vulkan/vulkan.h"

namespace VEE {
	VCommandBuffer::VCommandBuffer(VDevice* device, VCommandPool* commandPool) :m_Device(device)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool->GetHandle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		vkAllocateCommandBuffers(m_Device->GetLogicalDeviceHandle(), &allocInfo, &m_CommandBuffer);
		ASSERT(m_CommandBuffer != nullptr, "failed to Allocate CommandBuffer");
	}

	void VCommandBuffer::SetViewPort(VkViewport viewPort)
	{
		
		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewPort);
	}

	void VCommandBuffer::SetScissor(VkRect2D scissor)
	{
		
		vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
	}

	void VCommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

		ASSERT(result == VK_SUCCESS, "failed to Begin CommandBuffer");
	}

	void VCommandBuffer::End()
	{
		VkResult result = vkEndCommandBuffer(m_CommandBuffer);
		ASSERT(result == VK_SUCCESS, "failed to End CommandBuffer");
	}

	VCommandBuffer::~VCommandBuffer()
	{

	}

}
