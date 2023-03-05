#include "Renderpch.h"
#include "Renderpass.h"
#include "VDevice.h"
#include "CommandBuffer.h"
#include "FrameBuffer.h"
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

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		vkCreateRenderPass(m_Device->GetLogicalDeviceHandle(), &renderPassInfo, nullptr, &m_RenderPass);

		ASSERT(m_RenderPass != nullptr, "failed to create a Renderpass!");
	}

	void VRenderPass::Begin(VCommandBuffer* commandBuffer, VFrameBuffer* frameBuffer, VkExtent2D extent)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = frameBuffer->GetHandle();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = extent;
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(commandBuffer->GetHandle(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VRenderPass::End(VCommandBuffer* commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer->GetHandle());
	}

	VRenderPass::~VRenderPass()
	{
		vkDestroyRenderPass(m_Device->GetLogicalDeviceHandle(), m_RenderPass, nullptr);
	}

}