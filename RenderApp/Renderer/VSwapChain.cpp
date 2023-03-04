#include "Renderpch.h"
#include "VSwapChain.h"
#include "VWindowsSurface.h"
#include "VDevice.h"
#include "VGraphicsDevice.h"
#include "VertShader.h"
#include "FragShader.h"
#include "Renderpass.h"
#include "FrameBuffer.h"
#include "vulkan/vulkan.hpp"
#include <Core/Window.h>


namespace VEE {

	SwapChainSupportDetails* VSwapChain::s_Details = nullptr;

	VSwapChain::VSwapChain(VGraphicsDevice* device, VWindowsSurface* surface, Window* window) : m_Device(device)
	{
		const SwapChainSupportDetails* swapChainSupport = QuerySupport(device->GetPhysicalDeviceHandle(), surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport->formats);
		m_ImageFormat = surfaceFormat.format;
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport->presentModes);
		m_Extent = ChooseSwapExtent(swapChainSupport->capabilities, window);

		m_ImageCount = swapChainSupport->capabilities.minImageCount + 1;
		if (swapChainSupport->capabilities.maxImageCount > 0 && m_ImageCount > swapChainSupport->capabilities.maxImageCount) {
			m_ImageCount = swapChainSupport->capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface->GetHandle();
		createInfo.minImageCount = m_ImageCount;
		createInfo.imageFormat = m_ImageFormat;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = device->findQueueFamilies(device->GetPhysicalDeviceHandle(), (VkQueueFlagBits)(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT), surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport->capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		vkCreateSwapchainKHR(device->GetLogicalDeviceHandle(), &createInfo, nullptr, &m_SwapChain);
		ASSERT(m_SwapChain != nullptr, "failed to create SwapChain!");

		// get Images
		vkGetSwapchainImagesKHR(device->GetLogicalDeviceHandle(), m_SwapChain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(device->GetLogicalDeviceHandle(), m_SwapChain, &m_ImageCount, m_Images.data());
		// create Image views
		m_ImageViews.resize(m_Images.size());
		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_ImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			vkCreateImageView(m_Device->GetLogicalDeviceHandle(), &createInfo, nullptr, &m_ImageViews[i]);
			ASSERT(m_ImageViews[i] != nullptr, "failed to create Imageview!")
		}

		CreatGraphicsPipeline();

		m_FrameBuffers.resize(m_ImageViews.size());

		for (size_t i = 0; i < m_FrameBuffers.size(); i++) {
			VkImageView attachments[] = { m_ImageViews[i] };
			m_FrameBuffers[i] = new VFrameBuffer(m_Device, attachments, 1, m_RenderPass, m_Extent.width, m_Extent.height, 1);
		}

	}

	VSwapChain::~VSwapChain()
	{
		for (auto framebuffer : m_FrameBuffers) {
			delete framebuffer;
		}

		vkDestroyPipeline(m_Device->GetLogicalDeviceHandle(), m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device->GetLogicalDeviceHandle(), m_PipelineLayout, nullptr);
		delete m_RenderPass;

		for (auto imageView : m_ImageViews) {
			vkDestroyImageView(m_Device->GetLogicalDeviceHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device->GetLogicalDeviceHandle(), m_SwapChain, nullptr);
	}

	VkSurfaceFormatKHR VSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		// TODO: rank available formats and 
		return availableFormats[0];
	}

	VkPresentModeKHR VSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window)
	{
		#undef max // work around for max macro definition
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			WindowFrameBufferSize windowSize = window->GetFrameBufferSize();

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(windowSize.Width),
				static_cast<uint32_t>(windowSize.Height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	const SwapChainSupportDetails* VSwapChain::QuerySupport(VkPhysicalDevice device, VWindowsSurface* surface)
	{
		if (s_Details) return s_Details;
		
		s_Details = new SwapChainSupportDetails();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface->GetHandle(), &s_Details->capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetHandle(), &formatCount, nullptr);

		if (formatCount != 0) {
			s_Details->formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetHandle(), &formatCount, s_Details->formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetHandle(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			s_Details->presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetHandle(), &presentModeCount, s_Details->presentModes.data());
		}

		return s_Details;
	}

	const VEE::SwapChainSupportDetails* VSwapChain::ReQuerySupport(VDevice* device, VWindowsSurface* surface)
	{
		if (s_Details) {
			delete s_Details;
			s_Details = nullptr;
		}
		return QuerySupport(device->GetPhysicalDeviceHandle(), surface);
	}

	void VSwapChain::CreatGraphicsPipeline()
	{
		VVertexShader vertShader(m_Device, "RenderApp/Shaders/vert.spv");
		VFragmentShader fragShader(m_Device, "RenderApp/Shaders/frag.spv");

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShader.GetShaderStage(), fragShader.GetShaderStage() };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)m_Extent.width;
		viewport.height = (float)m_Extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_Extent;
		
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();
		
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		vkCreatePipelineLayout(m_Device->GetLogicalDeviceHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
		ASSERT(m_PipelineLayout != nullptr, "failed to create Pipeline Layout!");

		m_RenderPass = new VRenderPass(m_Device, m_ImageFormat);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = m_PipelineLayout;

		pipelineInfo.renderPass = m_RenderPass->GetHandle();
		pipelineInfo.subpass = 0;

		vkCreateGraphicsPipelines(m_Device->GetLogicalDeviceHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		ASSERT(m_PipelineLayout != nullptr, "failed to create Pipeline");
	}
}
