#include "Renderpch.h"
#include "Renderpass.h"
#include "VDevice.h"
#include "vulkan/vulkan.h"
/*
We need to specify how many color and depth buffers there will be, 
how many samples to use for each of them and 
how their contents should be handled throughout the rendering operations.
*/
namespace VEE {


	VRenderPass::VRenderPass(VDevice* device, VkFormat imageFormat) : m_Device(device)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	}

	VRenderPass::~VRenderPass()
	{
		vkDestroyRenderPass(m_Device->GetLogicalDeviceHandle(), m_RenderPass, nullptr);
	}

}