#pragma once
#include "VShader.h"
namespace VEE {
	class VVertexShader : public VShader {
	public:
		VVertexShader(VDevice*, const std::string&);
	protected:
		VkPipelineShaderStageCreateInfo CreateShaderStage(VkShaderModule) override;

	};
}