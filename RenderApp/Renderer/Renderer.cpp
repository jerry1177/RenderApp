#include "Renderpch.h"
#include "Renderer.h"
#include "vulkan/vulkan.hpp"

#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

void Renderer::Init()
{
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

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
