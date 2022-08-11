#include "DescriptorSets.hpp"

#include "Utilities.hpp"

namespace VE
{
	DescriptorSets::DescriptorSets(std::shared_ptr<Device> device, const DescriptorSetsSpec& descriptorSetsSpec)
		: m_Device(device), m_SetCount(descriptorSetsSpec.count)
	{
		m_DescriptorSetsLayoutBindings.resize(m_SetCount);
		for (size_t i = 0; i < m_DescriptorSetsLayoutBindings.size(); i++)
		{
			m_DescriptorSetsLayoutBindings[i].resize(descriptorSetsSpec.descriptorCounts[i]);
		}

		m_DescriptorSetsLayout.resize(m_SetCount);
		m_DescriptorSets.resize(m_SetCount);

		CreateSetsLayout(descriptorSetsSpec.descriptorSetsSpecs);
	}

	DescriptorSets::~DescriptorSets()
	{
		if (m_DescriptorSetsLayout.size() > 0)
		{
			for (size_t i = 0; i < m_DescriptorSetsLayout.size(); i++)
			{
				vkDestroyDescriptorSetLayout(m_Device->GetVkDevice(), m_DescriptorSetsLayout[i], nullptr);
			}
		}
		//if (m_DescriptorSets.size() > 0)
		//{
		//	vkFreeDescriptorSets(m_Device->GetVkDevice(), m_DescriptorPool->GetVkPool(), m_SetCount, m_DescriptorSets.data());
		//}
	}

	void DescriptorSets::CreateSetsLayout(const std::vector<std::vector<DescriptorSpec>>& descriptorSpecs)
	{
		for (size_t loop = 0; loop < m_DescriptorSetsLayoutBindings.size(); loop++)
		{
			for (size_t index = 0; index < m_DescriptorSetsLayoutBindings[loop].size(); index++)
			{
				m_DescriptorSetsLayoutBindings[loop][index].binding = static_cast<uint32_t>(index);
				m_DescriptorSetsLayoutBindings[loop][index].descriptorType = descriptorSpecs[loop][index].type;
				m_DescriptorSetsLayoutBindings[loop][index].descriptorCount = descriptorSpecs[loop][index].count;
				m_DescriptorSetsLayoutBindings[loop][index].stageFlags = descriptorSpecs[loop][index].stage;
				m_DescriptorSetsLayoutBindings[loop][index].pImmutableSamplers = nullptr;
			}
		}

		for (size_t i = 0; i < m_DescriptorSetsLayout.size(); i++)
		{
			VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutCreateInfo.bindingCount = static_cast<uint32_t>(m_DescriptorSetsLayoutBindings[i].size());
			layoutCreateInfo.pBindings = m_DescriptorSetsLayoutBindings[i].data();

			VK_CHECK(vkCreateDescriptorSetLayout(m_Device->GetVkDevice(), &layoutCreateInfo, nullptr, &m_DescriptorSetsLayout[i]))
		}
	}

	void DescriptorSets::AllocateDescriptors(DescriptorPool* pool)
	{
		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.descriptorPool = pool->GetVkPool();
		descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(m_DescriptorSets.size());
		descriptorSetAllocInfo.pSetLayouts = m_DescriptorSetsLayout.data();

		VK_CHECK(vkAllocateDescriptorSets(m_Device->GetVkDevice(), &descriptorSetAllocInfo, m_DescriptorSets.data()))
	}

	void DescriptorSets::UpdateDescriptors(const std::vector<BufferDescriptorInfo>& bufferDescInfos)
	{
		std::vector<VkWriteDescriptorSet> writeDescriptors;
		writeDescriptors.resize(bufferDescInfos.size());

		for (size_t i = 0; i < writeDescriptors.size(); i++)
		{
			VkWriteDescriptorSet writeDescriptorSet{};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.dstSet = bufferDescInfos[i].targetDescriptorSet;
			writeDescriptorSet.dstBinding = bufferDescInfos[i].targetDescriptorBinding;
			writeDescriptorSet.dstArrayElement = bufferDescInfos[i].targetArrayElement;
			writeDescriptorSet.descriptorType = bufferDescInfos[i].targetDescriptorType;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(bufferDescInfos[i].bufferInfos.size());
			writeDescriptorSet.pBufferInfo = bufferDescInfos[i].bufferInfos.data();

			writeDescriptors.push_back(writeDescriptorSet);
		}

		vkUpdateDescriptorSets(m_Device->GetVkDevice(), static_cast<uint32_t>(writeDescriptors.size()), writeDescriptors.data(), 0, nullptr);
	}

	void DescriptorSets::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const size_t set) const
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[set], 0, nullptr);
	}
}
