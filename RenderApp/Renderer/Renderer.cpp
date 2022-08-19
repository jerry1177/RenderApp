#include "Renderpch.h"
#include "Renderer.h"


#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>
#include "vulkan/vulkan.hpp"
#include "VulkanInstance.h"
void Renderer::Init()
{
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	createInstance();
	std::cout << extensionCount << " extensions supported\n";
	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	std::cout << test.length() << std::endl;
}

void Renderer::Render()
{
	
}

void Renderer::ShutDown()
{

}

void Renderer::createInstance()
{
	VulkanInstance temp;
	temp.CreateVKInstance(temp.GetRequiredExtensions());
	
}
