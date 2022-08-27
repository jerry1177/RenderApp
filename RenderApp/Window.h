#pragma once
struct GLFWwindow;
namespace VEE {
	class Event;
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height);
		~Window();

		GLFWwindow* GetGLFWWindow() const { return m_Window; }
		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }

		void SetEventCallback(const std::function<void(VEE::Event&)>& callback) { m_Data.EventCallback = callback; }
	private:
		GLFWwindow* m_Window;
		struct WindowData {
			uint32_t Width, Height;
			std::function<void(VEE::Event&)> EventCallback;
		};
		WindowData m_Data;
	};
}
