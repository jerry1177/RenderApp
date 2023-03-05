#pragma once
#include "VDevice.h"
namespace VEE {
	class Window;
	class VDeviceName;
	class VInstance;
	class VGraphicsDevice;
	class VWindowsSurface;
	class VSwapChain;
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
	private:
		void RecordCommandBuffer();
		void SetViewPort();
		void SetScissor();

	private:
		uint32_t m_MaxFramesInFlight = 2;
		Window* m_Window = nullptr;
		VInstance* m_Instance = nullptr;
		VGraphicsDevice* m_Device = nullptr;
		VWindowsSurface* m_Surface = nullptr;
		VSwapChain* m_SwapChain = nullptr;
		VCommandPool* m_CommandPool = nullptr;
		VCommandBuffer* m_CommandBuffer;
		VSemaphore* m_ImageAvailableSemaphore = nullptr;
		VSemaphore* m_RenderFinishedSemaphore = nullptr;
		VFence* m_InFlightFence = nullptr;
	};
}
