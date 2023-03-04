#include "Renderpch.h"
#include "CommandPool.h"
#include "VDevice.h"
#include "vulkan/vulkan.h"

namespace VEE {

	

	VCommandPool::VCommandPool(VDevice* device, uint32_t queueFamilyIndex) : m_Device(device)
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		vkCreateCommandPool(m_Device->GetLogicalDeviceHandle(), &poolInfo, nullptr, &m_CommandPool);
		ASSERT(m_CommandPool != nullptr, "failed to create a CommandPool!");
	}

	VCommandPool::~VCommandPool()
	{
		vkDestroyCommandPool(m_Device->GetLogicalDeviceHandle(), m_CommandPool, nullptr);
	}
}