#include "Renderpch.h"
#include "GraphicsApp.h"
#include "Renderer/VRenderer.h"
#include "Renderer/Buffer.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Core/Input.h"
#include <Core/TimeStep.h>
#include "Renderer/Buffer.h"
#include <GLFW/glfw3.h>
namespace VEE {
	GraphicsApp::GraphicsApp() : m_Running(false)
	{

	}

	void GraphicsApp::Init()
	{
		m_Window = new Window(800, 600);
		m_Window->SetEventCallback(BIND_EVENT_FN(GraphicsApp::OnEvent));


		if (!m_Renderer) m_Renderer = new VRenderer(m_Window);
#ifdef DEBUG
		const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
		};
		m_Renderer->EnableValidationLayers(validationLayers);
#endif // DEBUG
		std::vector<VEE::VBufferAttributeType> attributeTypes = { VEE::VBufferAttributeType::Vec2, VEE::VBufferAttributeType::Vec3 };
		VEE::VBufferLayout bufferLayout(attributeTypes, 0);
		
		m_Renderer->Init(bufferLayout);
		std::cout << "App Initialized!\n";
		std::cout << "Device count is: " << m_Renderer->GetDeviceNames().size() << std::endl;
		std::cout << "List of Devices:\n";
		for (const VDeviceName& deviceName : m_Renderer->GetDeviceNames()) {
			std::cout << deviceName.GetName() << std::endl;
		}
		std::cout << std::endl;

		

		const std::vector<VEE::Vertex> vertices = {
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		m_Renderer->SetVertexBuffer<VEE::Vertex>(vertices);

		m_Running = true;
	}

	void GraphicsApp::Run()
	{
		std::cout << "App Is Running!\n\n";
		float previousTime = 0;

		float colorChange = 0, rate = 1;
		while (m_Running) {
			if (m_Window->isMinimized()) {
				m_Window->Update();
				continue;
			}
			float currentTime = glfwGetTime();
			TimeStep timeStep = currentTime - previousTime;
			previousTime = currentTime;
			std::cout << "TimeStep: " << 1.0f/timeStep << "ms\n";

			m_Window->Update();
			//std::cout << "Current Processing Frame: " << m_Renderer->GetCurrentFrame() + 1 << std::endl;
			
						
			m_Renderer->Render();
		}
	}

	void GraphicsApp::ShutDown()
	{
		m_Renderer->ShutDown();
		delete m_Window;
		std::cout << "App Has Shut Down!\n";

	}

	bool GraphicsApp::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool GraphicsApp::OnWIndowResize(WindowResizeEvent& e)
	{
		
		if (m_Renderer)
			m_Renderer->FrameBufferResized();
		return false;
	}

	void GraphicsApp::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(GraphicsApp::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(GraphicsApp::OnWIndowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
}