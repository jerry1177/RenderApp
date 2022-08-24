#pragma once

class Window;
class VInstance;
class VRenderer
{
public:
	VRenderer(Window* window) : m_Window(window) {}
	void Init();
	void Render();
	void ShutDown();
	void EnableValidationLayers(const std::vector<const char*>&);
private:
	
private:
	Window* m_Window = nullptr;

	VInstance* m_Instance = nullptr;
};

