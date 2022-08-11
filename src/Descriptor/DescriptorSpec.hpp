#pragma once

#include <vulkan/vulkan.h>

namespace VE
{
	struct DescriptorSpec
	{
		VkDescriptorType		type;
		VkShaderStageFlagBits	stage;
		uint32_t				count;
	};

	struct DescriptorSetsSpec
	{
		uint32_t									count;
		std::vector<uint32_t>						descriptorCounts;
		std::vector<std::vector<DescriptorSpec>>	descriptorSetsSpecs;
	};
}