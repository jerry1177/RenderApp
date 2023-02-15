#include "Renderpch.h"
#include "FragShader.h"
#include "vulkan/vulkan.h"

namespace VEE {

	VFragmentShader::VFragmentShader(VDevice* device, const std::string& path) : VShader(device, path)
	{
		m_ShaderStage = CreateShaderStage(m_ShaderModule);
	}

	VkPipelineShaderStageCreateInfo VFragmentShader::CreateShaderStage(VkShaderModule)
	{
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = m_ShaderModule;
		fragShaderStageInfo.pName = "main";

		return fragShaderStageInfo;
	}

}