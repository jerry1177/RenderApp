#pragma once
#include "vulkan/vulkan.hpp"
typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkExtent2D;
namespace VEE {
	class VDevice;
	class VGraphicsDevice;
	class VWindowsSurface;
	class Window;

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
	};
}