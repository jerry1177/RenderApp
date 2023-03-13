#pragma once
#include "VDevice.h"
#include "Buffer.h"
#include "glm/glm.hpp"
namespace VEE {
	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;
	};
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
	class VBufferLayout;
	class VVertexBuffer;
	class VSemaphore;
	class VFence;

	class VRenderer
	{
	public:
		VRenderer(Window* window) : m_Window(window) {}
		void Init(const VBufferLayout& bufferLayout);
		template<typename T>
		void SetVertexBuffer(const std::vector<T>& vertices);
		
		void RemoveVertexBuffer();
		void Render();
		void ShutDown();
		void EnableValidationLayers(const std::vector<const char*>&);
		const std::vector<VDeviceName>& GetDeviceNames();
		uint32_t GetCurrentFrame() const { return m_CurrentFrame; }
		const VDevice* GetDevice() const { return (VDevice*)m_Device; }
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
		VVertexBuffer* m_VertexBuffer = nullptr;
	};

	template<typename T>
	void VRenderer::SetVertexBuffer(const std::vector<T>& vertices)
	{

		m_VertexBuffer = VVertexBuffer::Create<T>(GetDevice(), vertices);
		m_VertexBuffer->CopyVerticesToGPU<T>(vertices);
	}

	

}
