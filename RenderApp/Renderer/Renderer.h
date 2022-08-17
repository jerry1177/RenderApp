#pragma once
class Window;
class Renderer
{
public:
	Renderer(Window* window) : m_Window(window) {}
	void Init();
	void Render();
	void ShutDown();
private:
	Window* m_Window = nullptr;
};

