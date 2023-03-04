#pragma once
#include "vulkan/vulkan.hpp"
typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPipeline_T* VkPipeline;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkViewport;
typedef struct VkExtent2D;
namespace VEE {
	class VDevice;
	class VGraphicsDevice;
	class VWindowsSurface;
	class VRenderPass;
	class VFrameBuffer;
	class Window;
	class VCommandBuffer;

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VSwapChain
	{
	public:
		VSwapChain(VGraphicsDevice*, VWindowsSurface*, Window*);
		VkSwapchainKHR GetHandle() { return m_SwapChain; }
		void BeginRenderPass(VCommandBuffer*, uint32_t);
		void EndRenderPass(VCommandBuffer*);
		void BindGraphicsPipeline(VCommandBuffer*);
		VkExtent2D GetExtent() const { return m_Extent; }
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>&);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR&, Window*);
		static const SwapChainSupportDetails* QuerySupport(VkPhysicalDevice, VWindowsSurface*);
		static const SwapChainSupportDetails* ReQuerySupport(VDevice*, VWindowsSurface*);
		~VSwapChain();
	private:
		void CreatGraphicsPipeline();
	private:
		static SwapChainSupportDetails* s_Details;
		uint32_t m_ImageCount;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_Extent;
		VkSwapchainKHR m_SwapChain = nullptr;
		VDevice* m_Device = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VRenderPass* m_RenderPass = nullptr;
		VkPipeline m_GraphicsPipeline = nullptr;
		std::vector<VFrameBuffer*> m_FrameBuffers;
	};
}