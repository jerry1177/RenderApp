#pragma once
#include "vulkan/vulkan.hpp"
typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPipeline_T* VkPipeline;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkViewport;
typedef struct VkExtent2D;
enum VkResult;
namespace VEE {
	class VDevice;
	class VGraphicsDevice;
	class VWindowsSurface;
	class VRenderPass;
	class VFrameBuffer;
	class Window;
	class VCommandBuffer;
	class VSemaphore;

	

	class VSwapChain
	{
	public:
		VSwapChain(VGraphicsDevice*, VWindowsSurface*, Window*, VRenderPass*);
		VkSwapchainKHR GetHandle() { return m_SwapChain; }
		VkResult AcquireNextImageIndex(std::shared_ptr<VSemaphore>&, uint32_t&);
		VkExtent2D GetExtent() const { return m_Extent; }
		VkFormat GetImageFormat() const { return m_ImageFormat; }
		VFrameBuffer* GetFrameBufferAt(uint32_t index) { return m_FrameBuffers[index]; }
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>&);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR&, Window*);
		~VSwapChain();
	private:
	private:
		//static SurfaceSupportDetails* s_Details;
		//VSemaphore* m_ImageAvailableSemaphore = nullptr;
		uint32_t m_ImageCount;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_Extent;
		VkSwapchainKHR m_SwapChain = nullptr;
		VDevice* m_Device = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipeline m_GraphicsPipeline = nullptr;
		std::vector<VFrameBuffer*> m_FrameBuffers;
	};
}