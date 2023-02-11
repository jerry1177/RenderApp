#pragma once
#include "vulkan/vulkan.hpp"
namespace VEE {
	class VDevice;
	class VGraphicsDevice;
	class VWindowsSurface;
	class Window;
	typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
	typedef struct VkPhysicalDevice_T* VkPhysicalDevice;

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
		static SwapChainSupportDetails* s_Details;
		uint32_t m_ImageCount;
		VkSwapchainKHR m_SwapChain = nullptr;
		VDevice* m_Device = nullptr;
	};
}