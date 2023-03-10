#pragma once
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkExtent2D;
enum VkFormat;

namespace VEE {
	class VDevice;
	class VCommandBuffer;
	class VFrameBuffer;
	class VRenderPass
	{
	public:
		VRenderPass(VDevice*, VkFormat);
		void Begin(std::shared_ptr<VCommandBuffer>&, VFrameBuffer*, VkExtent2D);
		void End(std::shared_ptr<VCommandBuffer>&);
		VkRenderPass GetHandle() { return m_RenderPass; }
		~VRenderPass();

	private:
		VkRenderPass m_RenderPass = nullptr;
		VDevice* m_Device = nullptr;
	};

}