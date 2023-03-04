#pragma once
#include "VDevice.h"
namespace VEE {
	class Window;
	class VInstance;
	class VDevice;
	class VWindowsSurface;
	class VSwapChain;
	class VCommandPool;
	class VCommandBuffer;

	class VRenderer
	{
	public:
		VRenderer(Window* window) : m_Window(window) {}
		void Init();
		void Render();
		void ShutDown();
		void EnableValidationLayers(const std::vector<const char*>&);
		const std::vector<VDeviceName>& GetDeviceNames() { return m_Device->GetDeviceNames(); }
	private:
		void RecordCommandBuffer();
		void SetViewPort();
		void SetScissor();

	private:
		Window* m_Window = nullptr;
		VInstance* m_Instance = nullptr;
		VDevice* m_Device = nullptr;
		VWindowsSurface* m_Surface = nullptr;
		VSwapChain* m_SwapChain = nullptr;
		VCommandPool* m_CommandPool = nullptr;
		VCommandBuffer* m_CommandBuffer = nullptr;
	};
}
