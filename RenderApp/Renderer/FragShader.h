#pragma once
#include "VShader.h"
namespace VEE {
	class VFragmentShader : public VShader {
	public:
		VFragmentShader(VDevice*, const std::string&);
	protected:

		virtual VkPipelineShaderStageCreateInfo CreateShaderStage(VkShaderModule) override;

	};
}