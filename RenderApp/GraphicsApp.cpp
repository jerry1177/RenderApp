#include "Renderpch.h"
#include "GraphicsApp.h"



#include <iostream>
#include <GLFW/glfw3.h>

GraphicsApp::GraphicsApp()
{
	
}

void GraphicsApp::Init()
{
	glfwInit();

	m_Window = new Window(800, 600);
	
	if (!m_Renderer) m_Renderer = new Renderer(m_Window);
	m_Renderer->Init();
	std::cout << "App Initialized!\n";
}

void GraphicsApp::Run()
{
	std::cout << "App Is Running!\n";
	
	while (!glfwWindowShouldClose(m_Window->GetGLFWWindow())) {
		glfwPollEvents();
		m_Renderer->Render();
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
