#include "Renderpch.h"
#include "VShader.h"
#include "VDevice.h"
#include "VFileReader.h"
#include "vulkan/vulkan.h"

namespace VEE {


	VShader::VShader(VDevice* device, const std::string& path) : m_Device(device)
	{
		m_ShaderCode = VFileReader::readFile(path);
		m_ShaderModule = CreateShaderModule(m_ShaderCode);
		//m_ShaderStage = CreateShaderStage(m_ShaderModule);
	}

	VShader::~VShader()
	{
		vkDestroyShaderModule(m_Device->GetLogicalDeviceHandle(), m_ShaderModule, nullptr);
	}

	VkShaderModule VShader::CreateShaderModule(const std::vector<char>& shaderCode)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
		VkShaderModule shaderModule;
		vkCreateShaderModule(m_Device->GetLogicalDeviceHandle(), &createInfo, nullptr, &shaderModule);
		ASSERT(shaderModule != nullptr, "failed to create Shader!");
		return shaderModule;
	}

}