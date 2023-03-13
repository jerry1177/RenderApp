#pragma once

typedef struct VkPipeline_T* VkPipeline;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkExtent2D;
namespace VEE {
	class VDevice;
	class VRenderPass;
	class VCommandBuffer;
	class VBufferLayout;

	class VGraphicsPipeline
	{
	public:
		VGraphicsPipeline(VDevice* device, VRenderPass* renderPass, VkExtent2D extent, const VBufferLayout& bufferLayout);
		VkPipeline GetHandle() { return m_GraphicsPipeline; }
		void Bind(std::shared_ptr<VCommandBuffer>& commandBuffer);
		~VGraphicsPipeline();
	private:
		VDevice* m_Device = nullptr;
		VkPipelineLayout m_Layout = nullptr;
		VkPipeline m_GraphicsPipeline = nullptr;
	};
}

