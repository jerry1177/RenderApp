#include "Renderpch.h"
#include "Semaphore.h"
#include "VDevice.h"
#include "vulkan/vulkan.h"

namespace VEE {


	VSemaphore::VSemaphore(VDevice* device) : m_Device(device)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(m_Device->GetLogicalDeviceHandle(), &semaphoreInfo, nullptr, &m_Semaphore);
		ASSERT(m_Semaphore != nullptr, "failed to create a Semaphore!");
	}

	VSemaphore::~VSemaphore()
	{
		vkDestroySemaphore(m_Device->GetLogicalDeviceHandle(), m_Semaphore, nullptr);
	}

}