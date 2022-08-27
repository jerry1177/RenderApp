#include "Renderpch.h"
#include "GraphicsApp.h"
#include "Renderer/VRenderer.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include "Events/ApplicationEvent.h"
#include "Input.h"
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
			glfwPollEvents();
			m_Renderer->Render();

			if (Input::IsKeyReleased(m_Window, GLFW_KEY_E)) {
				std::cout << "Key E is pressed!\n";
			}
		}



	}

	void GraphicsApp::ShutDown()
	{
		m_Renderer->ShutDown();
		delete m_Window;
		m_Window = nullptr;
		glfwTerminate();
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