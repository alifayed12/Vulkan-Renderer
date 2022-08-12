#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"
#include "Buffer/Buffer.hpp"

#include <memory>
#include <unordered_map>

namespace VE
{
	struct BufferDescriptorInfo 
	{
		uint32_t							targetDescriptorBinding;
		uint32_t							targetArrayElement;
		VkDescriptorType					targetDescriptorType;
		std::vector<VkDescriptorBufferInfo> bufferInfos;
	};

	class DescriptorSet
	{
	public:
		DescriptorSet(Device* device);
		~DescriptorSet();
	public:
		inline const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }
	public:
		void Create(const std::vector<uint32_t>& descriptorSetInfo);
		void Update(const uint32_t set, const uint32_t binding, const Buffer* buffer);
		void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t set);
	private:
		void CreateDescriptorPool(const uint32_t numUniformDescriptors);
	private:
		Device*								m_Device;
		VkDescriptorPool					m_DescriptorPool;
		std::vector<VkDescriptorSetLayout>	m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet>		m_DescriptorSets;
	};
}

