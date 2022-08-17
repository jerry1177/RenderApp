#pragma once
#include "App.h"
#define GLFW_INCLUDE_VULKAN
#include "Window.h"
#include "Renderer/Renderer.h"
class GraphicsApp : public App
{
public:
	GraphicsApp();
	virtual void Init() override;
	virtual void Run() override;
	virtual void ShutDown() override;
private:
	Window* m_Window = nullptr;
	Renderer* m_Renderer = nullptr;
};

