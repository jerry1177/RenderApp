#include "Renderpch.h"
#include "GraphicsApp.h"
#include "Renderer/VRenderer.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Core/Input.h"
#include <Core/TimeStep.h>
namespace VEE {
	GraphicsApp::GraphicsApp() :m_Running(false)
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
		m_Renderer->Init();
		std::cout << "App Initialized!\n";
		m_Running = true;
	}

	void GraphicsApp::Run()
	{
		std::cout << "App Is Running!\n";

		while (m_Running) {

			m_Window->Update();
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