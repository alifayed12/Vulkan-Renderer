#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"
#include "DescriptorSpec.hpp"
#include "DescriptorPool.hpp"

#include <memory>
#include <vector>
#include <optional>

namespace VE
{
	struct BufferDescriptorInfo {
		VkDescriptorSet						targetDescriptorSet;
		uint32_t							targetDescriptorBinding;
		uint32_t							targetArrayElement;
		VkDescriptorType					targetDescriptorType;
		std::vector<VkDescriptorBufferInfo> bufferInfos;
	};

	class DescriptorSets
	{
	public:
		DescriptorSets(std::shared_ptr<Device> device, const DescriptorSetsSpec& descriptorSetsSpec);
		~DescriptorSets();

		DescriptorSets(const DescriptorSets& otherDescriptorSet) = delete;
		DescriptorSets& operator=(const DescriptorSets& otherDescriptorSet) = delete;
	public:
		inline const std::vector<VkDescriptorSetLayout>& GetVkDescriptorSetLayouts() const { return m_DescriptorSetsLayout; }
		inline const std::vector<VkDescriptorSet>& GetVkDescriptorSets() const { return m_DescriptorSets; }
	public:
		void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const size_t set) const;
	public:
		void UpdateDescriptors(const std::vector<BufferDescriptorInfo>& bufferDescInfos);
		void AllocateDescriptors(DescriptorPool* pool);
	private:
		void CreateSetsLayout(const std::vector<std::vector<DescriptorSpec>>& descriptorSpecs);
	private:
		std::shared_ptr<Device>									m_Device;
		uint32_t												m_SetCount;
		std::vector<std::vector<VkDescriptorSetLayoutBinding>>	m_DescriptorSetsLayoutBindings;
		std::vector<VkDescriptorSetLayout>						m_DescriptorSetsLayout;
		std::vector<VkDescriptorSet>							m_DescriptorSets;
	};
}

