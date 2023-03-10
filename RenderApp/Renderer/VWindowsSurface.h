#pragma once
#include "vulkan/vulkan.h"
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
namespace VEE {
	class VDevice;

	struct SurfaceSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VInstance;
	class Window;
	class VWindowsSurface {
	public:
		VWindowsSurface(VInstance*, Window*);
		~VWindowsSurface();
		VkSurfaceKHR GetHandle() const { return m_Surface; }
		const SurfaceSupportDetails* QuerySupport(VkPhysicalDevice device);
		const SurfaceSupportDetails* QuerySupport(VDevice* device);
		const SurfaceSupportDetails* ReQuerySupport(VDevice* device, VWindowsSurface* surface);

	private:
		VkSurfaceKHR m_Surface;
		VInstance* m_Instance;
		static SurfaceSupportDetails* s_SupportDetails;
	};
}