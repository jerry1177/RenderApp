#include "Renderpch.h"
#include "VertShader.h"
#include "VDevice.h"
#include "vulkan/vulkan.h"
namespace VEE {

	VVertexShader::VVertexShader(VDevice* device, const std::string& path) : VShader(device, path)
	{
		m_ShaderStage = CreateShaderStage(m_ShaderModule);
	}

	VkPipelineShaderStageCreateInfo VVertexShader::CreateShaderStage(VkShaderModule shaderModule)
	{
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = shaderModule;
		vertShaderStageInfo.pName = "main";

		return vertShaderStageInfo;
	}

}