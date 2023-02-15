#pragma once
#include "vulkan/vulkan.h"

namespace VEE {
	class VDevice;
	class VShader
	{
	public:
		VShader(VDevice*, const std::string&);
		VkShaderModule GetShaderModule() { return m_ShaderModule; }
		VkPipelineShaderStageCreateInfo GetShaderStage() { return m_ShaderStage; }
		
		virtual ~VShader();

	protected:
		virtual VkShaderModule CreateShaderModule(const std::vector<char>&);
		virtual VkPipelineShaderStageCreateInfo CreateShaderStage(VkShaderModule) = 0;
	protected:
		VDevice* m_Device = nullptr;
		std::vector<char> m_ShaderCode;
		VkShaderModule m_ShaderModule;
		VkPipelineShaderStageCreateInfo m_ShaderStage;

	};
}