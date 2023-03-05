#include "Renderpch.h"
#include "Fence.h"
#include "VDevice.h"
#include "vulkan/vulkan.h"

namespace VEE {

	VFence::VFence(VDevice* device, VkFenceCreateFlags flags) : m_Device(device)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = flags;
		vkCreateFence(m_Device->GetLogicalDeviceHandle(), &fenceInfo, nullptr, &m_Fence);
		ASSERT(m_Fence != nullptr, "failed to create a Fence!");
	}

	VFence::~VFence()
	{
		vkDestroyFence(m_Device->GetLogicalDeviceHandle(), m_Fence, nullptr);
	}
}