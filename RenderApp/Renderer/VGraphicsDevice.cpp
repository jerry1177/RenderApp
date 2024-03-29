#include "Renderpch.h"
#include "VGraphicsDevice.h"
#include "VInstance.h"
#include "VWindowsSurface.h"
#include "VSwapChain.h"
#include "vulkan/vulkan.h"
namespace VEE {
	VGraphicsDevice::VGraphicsDevice(VInstance* instance, std::vector<const char*>& requiredExtensions , VWindowsSurface* surface) : VDevice(instance, requiredExtensions)
	{
		PickPhysicalDevice(GetPhisicalDevices(), surface);
		ASSERT(m_PhysicalDevice != nullptr, "failed to find a suitable GPU!");

		CreatLogicalDevice(m_PhysicalDevice, surface);

		m_Indices = findQueueFamilies(m_PhysicalDevice, VK_QUEUE_GRAPHICS_BIT, surface);

		vkGetDeviceQueue(m_LogicalDevice, m_Indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		ASSERT(m_GraphicsQueue != nullptr, "failed to get graphics Queue!");
		vkGetDeviceQueue(m_LogicalDevice, m_Indices.presentFamily.value(), 0, &m_PresentQueue);
		ASSERT(m_PresentQueue != nullptr, "failed to get present Queue!");

	}

	void VGraphicsDevice::PickPhysicalDevice(const std::vector<VkPhysicalDevice>& devices, VWindowsSurface* surface) // just pick first suitable device for now
	{
		struct SuitableDevice {
			VkPhysicalDevice device;
			float score;
			bool operator()(const SuitableDevice& left, const SuitableDevice& right) const {
				return left.score < right.score;
			}
		};
		std::priority_queue<SuitableDevice, std::vector<SuitableDevice>, SuitableDevice> suitableDevices;
		for (const auto& device : devices) {
			if (IsDeviceSuitable(device, surface)) {
				suitableDevices.push({ device, ScoreDevice(device) });
				
			}
		}

		m_PhysicalDevice = suitableDevices.top().device;
		uint32_t queueFamilyCount = 0;
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);
		
	}

	bool VGraphicsDevice::IsDeviceSuitable(VkPhysicalDevice device, VWindowsSurface* surface)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT, surface);
		bool isExtensionsSupported = CheckDeviceExtensionSupport(device);
		bool swapChainAdaquate = false;
		if (isExtensionsSupported) {
			const SurfaceSupportDetails* surfaceDetails = surface->QuerySupport(device);
			swapChainAdaquate = !surfaceDetails->formats.empty() && !surfaceDetails->presentModes.empty();
		}

		return indices.graphicsFamily.has_value() && indices.presentFamily.has_value() && isExtensionsSupported && swapChainAdaquate;
	}

	QueueFamilyIndices VGraphicsDevice::findQueueFamilies(VkPhysicalDevice device, VkQueueFlagBits queueFlags, VWindowsSurface* surface)
	{
		QueueFamilyIndices indices;
		// Logic to find queue family indices to populate struct with

		std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilies(device);

		int index = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = index;
			}
			if (queueFamily.queueFlags & queueFlags & VK_QUEUE_TRANSFER_BIT) {
				indices.transferFamily = index;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface->GetHandle(), &presentSupport);
			if (presentSupport) {
				indices.presentFamily = index;
			}
			index++;
		}
		return indices;
	}

	void VGraphicsDevice::CreatLogicalDevice(VkPhysicalDevice device, VWindowsSurface* surface)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, VK_QUEUE_GRAPHICS_BIT, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_RequiredDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_RequiredDeviceExtensions.data();

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = 0;


		if (m_VInstance->HasEnabledValidationLayers()) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_VInstance->GetValidationLayers().size());
			createInfo.ppEnabledLayerNames = m_VInstance->GetValidationLayers().data();
		}
		vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);
		ASSERT(m_LogicalDevice != nullptr, "failed to create logical device!");
		
	}
}
