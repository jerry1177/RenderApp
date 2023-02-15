#pragma once
typedef struct VkRenderPass_T* VkRenderPass;
enum VkFormat;

namespace VEE {
	class VDevice;
	class VRenderPass
	{
	public:
		VRenderPass(VDevice*, VkFormat);
		VkRenderPass GetHandle() { return m_RenderPass; }
		~VRenderPass();

	private:
		VkRenderPass m_RenderPass = nullptr;
		VDevice* m_Device = nullptr;
	};

}