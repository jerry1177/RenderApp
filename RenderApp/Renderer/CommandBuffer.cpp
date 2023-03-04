
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
	}

	VCommandBuffer::~VCommandBuffer()
	{

	}

}
