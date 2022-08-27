#pragma once
#include "App.h"
#include "LayerStack.h"
namespace VEE {
	class Window;
	class VRenderer;
	class LayerStack;
	class Event;
	class WindowResizeEvent;
	class WindowCloseEvent;

	class GraphicsApp : public App
	{
	public:
		GraphicsApp();
		virtual void Init() override;
		virtual void OnEvent(Event&) override;
		virtual void Run() override;
		virtual void ShutDown() override;
	private:
		bool OnWindowClose(WindowCloseEvent&);
		bool OnWIndowResize(WindowResizeEvent&);
	private:
		Window* m_Window = nullptr;
		VRenderer* m_Renderer = nullptr;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		bool m_Running;
	};
}
