#pragma once
#include "VDevice.h"
namespace VEE {
	class Window;
	class VDeviceName;
	class VInstance;
	class VGraphicsDevice;
	class VWindowsSurface;
	class VSwapChain;
	class VRenderPass;
	class VGraphicsPipeline;
	class VCommandPool;
	class VCommandBuffer;
	class VSemaphore;
	class VFence;

	class VRenderer
	{
	public:
		VRenderer(Window* window) : m_Window(window) {}
		void Init();
		void Render();
		void ShutDown();
		void EnableValidationLayers(const std::vector<const char*>&);
		const std::vector<VDeviceName>& GetDeviceNames();
		uint32_t GetCurrentFrame() const { return m_CurrentFrame; }
		void RecreateSwapChain();
		void FrameBufferResized() { m_FrameBufferResized = true; }
	private:
		void SetViewPort();
		void SetScissor();

	private:
		uint32_t m_MaxFramesInFlight = 2;
		bool m_FrameBufferResized = false;
		Window* m_Window = nullptr;
		VInstance* m_Instance = nullptr;
		VGraphicsDevice* m_Device = nullptr;
		VWindowsSurface* m_Surface = nullptr;
		VRenderPass* m_RenderPass = nullptr;
		VGraphicsPipeline* m_GraphicsPipeline = nullptr;
		VSwapChain* m_SwapChain = nullptr;
		VCommandPool* m_CommandPool = nullptr;
		std::vector<std::shared_ptr<VCommandBuffer>> m_CommandBuffers;
		std::vector< std::shared_ptr<VSemaphore>> m_ImageAvailableSemaphores;
		std::vector< std::shared_ptr<VSemaphore>> m_RenderFinishedSemaphores;
		std::vector< std::shared_ptr<VFence>> m_InFlightFences;
		uint32_t m_CurrentFrame = 0;
	};
}
