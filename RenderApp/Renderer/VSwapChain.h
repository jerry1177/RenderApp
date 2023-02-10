#pragma once
#include "vulkan/vulkan.hpp"
namespace VEE {
	class VDevice;
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
		VSwapChain();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>&);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR&, Window*);
		static const SwapChainSupportDetails* QuerySupport(VDevice*, VWindowsSurface*);
		static const SwapChainSupportDetails* ReQuerySupport(VDevice*, VWindowsSurface*);
		~VSwapChain();

	private:
		static SwapChainSupportDetails* s_Details;
	};

	VSwapChain::VSwapChain()
	{
	}

	VSwapChain::~VSwapChain()
	{
	}
}