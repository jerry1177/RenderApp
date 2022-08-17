#pragma once

struct GLFWwindow;
class Window
{
public :
	Window(uint32_t width, uint32_t height);
	~Window();
	GLFWwindow* GetGLFWWindow() const { return m_Window; }
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
private:
	GLFWwindow* m_Window;
	uint32_t m_Width;
	uint32_t m_Height;
};

