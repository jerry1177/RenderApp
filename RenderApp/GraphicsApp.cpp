#include "GraphicsApp.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <iostream>

void GraphicsApp::Init()
{

	std::cout << "App Initialized!\n";
}

void GraphicsApp::Run()
{
	std::cout << "App Is Running!\n";
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported\n";


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	
}

void GraphicsApp::ShutDown()
{
	std::cout << "App Has Shut Down!\n";
}
