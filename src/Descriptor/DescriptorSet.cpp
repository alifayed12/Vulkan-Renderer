#include "DescriptorSet.hpp"

#include "Utilities.hpp"

#include <vector>

namespace VE
{
	DescriptorSet::DescriptorSet(std::shared_ptr<Device> device)
		:	m_Device(device), m_DescriptorPool(VK_NULL_HANDLE)
	{
	}

	DescriptorSet::~DescriptorSet()
	{
		if (m_DescriptorSetLayouts.size() > 0)
		{
			for (size_t i = 0; i < m_DescriptorSetLayouts.size(); i++)
			{
				vkDestroyDescriptorSetLayout(m_Device->GetVkDevice(), m_DescriptorSetLayouts[i], nullptr);
			}
		}

		if (m_DescriptorPool)
		{
			vkDestroyDescriptorPool(m_Device->GetVkDevice(), m_DescriptorPool, nullptr);
		}
	}

	void DescriptorSet::CreateDescriptorPool(const uint32_t numUniformDescriptors)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = numUniformDescriptors;

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.maxSets = 3;
		poolCreateInfo.poolSizeCount = 1;
		poolCreateInfo.pPoolSizes = &poolSize;

		VK_CHECK(vkCreateDescriptorPool(m_Device->GetVkDevice(), &poolCreateInfo, nullptr, &m_DescriptorPool))
	}

	void DescriptorSet::Create(const std::vector<uint32_t>& descriptorSetInfo)
	{
		CreateDescriptorPool(static_cast<uint32_t>(descriptorSetInfo.size()));

		std::vector<std::vector<VkDescriptorSetLayoutBinding>> layoutBindings(descriptorSetInfo.size());
		for (size_t i = 0; i < descriptorSetInfo.size(); i++)
		{
			layoutBindings[i].resize(descriptorSetInfo[i]);
			for (size_t j = 0; j < descriptorSetInfo[i]; j++)
			{
				layoutBindings[i][j].binding = static_cast<uint32_t>(j);
				layoutBindings[i][j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layoutBindings[i][j].descriptorCount = 1;
				layoutBindings[i][j].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layoutBindings[i][j].pImmutableSamplers = nullptr;
			}
		}

		m_DescriptorSetLayouts.resize(descriptorSetInfo.size());
		for (size_t i = 0; i < m_DescriptorSetLayouts.size(); i++)
		{
			VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings[i].size());
			layoutCreateInfo.pBindings = layoutBindings[i].data();

			VK_CHECK(vkCreateDescriptorSetLayout(m_Device->GetVkDevice(), &layoutCreateInfo, nullptr, &m_DescriptorSetLayouts[i]))
		}

		m_DescriptorSets.resize(m_DescriptorSetLayouts.size());

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.descriptorPool = m_DescriptorPool;
		descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(m_DescriptorSets.size());
		descriptorSetAllocInfo.pSetLayouts = m_DescriptorSetLayouts.data();

		VK_CHECK(vkAllocateDescriptorSets(m_Device->GetVkDevice(), &descriptorSetAllocInfo, m_DescriptorSets.data()))
	}

	void DescriptorSet::Update(const uint32_t set, const uint32_t binding, const Buffer* buffer)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = buffer->GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = buffer->GetDataSize();

		VkWriteDescriptorSet writeDescriptorSet{};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = m_DescriptorSets[set];
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.dstArrayElement = 0;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_Device->GetVkDevice(), 1, &writeDescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t set)
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[set], 0, 0);
	}
}
