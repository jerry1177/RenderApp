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
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		vkCreateRenderPass(m_Device->GetLogicalDeviceHandle(), &renderPassInfo, nullptr, &m_RenderPass);

		ASSERT(m_RenderPass != nullptr, "failed to create a Renderpass!");
	}

	VRenderPass::~VRenderPass()
	{
		vkDestroyRenderPass(m_Device->GetLogicalDeviceHandle(), m_RenderPass, nullptr);
	}

}