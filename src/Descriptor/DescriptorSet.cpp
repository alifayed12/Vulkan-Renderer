#include "DescriptorSet.hpp"

#include "Buffer/UniformBuffer.hpp"
#include "Utilities.hpp"

#include <vector>

namespace VE
{
	DescriptorSet::DescriptorSet(Device* device)
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

	void DescriptorSet::CreateDescriptorPool(const uint32_t numUniformDescriptors, const uint32_t numSamplerDescriptors)
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes;

		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = numUniformDescriptors;

		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = numSamplerDescriptors;

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.maxSets = 3;
		poolCreateInfo.poolSizeCount = poolSizes.size();
		poolCreateInfo.pPoolSizes = poolSizes.data();

		VK_CHECK(vkCreateDescriptorPool(m_Device->GetVkDevice(), &poolCreateInfo, nullptr, &m_DescriptorPool))
	}

	// vector size represents sets. vector content (uint32_t) represents descriptors in set.
	// currently this just represents uniform descriptors. need to change to allow for 
	// descriptors and samplers
	void DescriptorSet::Create(const std::vector<DescriptorSetInfo>& descriptorSetInfo)
	{
		/*
			numUniforms
			numSamplers
		*/
		uint32_t numUniforms{}, numSamplers{};
		for (size_t i = 0; i < descriptorSetInfo.size(); i++)
		{
			numUniforms += descriptorSetInfo[i].numUniforms;
			numSamplers += descriptorSetInfo[i].numSamplers;
		}
		CreateDescriptorPool(numUniforms, numSamplers);


		for (size_t i = 0; i < descriptorSetInfo.size(); i++)
		{
			size_t k = 0;
			for (size_t j = 0; j < descriptorSetInfo[i].numUniforms; j++)
			{
				m_DescriptorBuffers.insert({ Key(i, k), std::make_unique<UniformBuffer>(m_Device, sizeof(GlobalUniform)) });
				k++;
			}
			for (size_t j = 0; j < descriptorSetInfo[i].numSamplers; j++)
			{
				m_DescriptorImages.insert({ Key(i, k), std::make_unique<Texture>(m_Device, "C:\\Users\\alifa\\Downloads\\stanfordbunny.jpg") });
				k++;
			}
		}

		std::vector<std::vector<VkDescriptorSetLayoutBinding>> layoutBindings(descriptorSetInfo.size());
		for (size_t i = 0; i < descriptorSetInfo.size(); i++)
		{
			uint32_t numDescUniforms = descriptorSetInfo[i].numUniforms;
			uint32_t numDescSamplers = descriptorSetInfo[i].numSamplers;
			uint32_t totalDescriptors = numDescUniforms + numDescSamplers;

			layoutBindings[i].resize(totalDescriptors);
			size_t k = 0;
			for (size_t j = 0; j < numDescUniforms; j++)
			{
				layoutBindings[i][k].binding = static_cast<uint32_t>(k);
				layoutBindings[i][k].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layoutBindings[i][k].descriptorCount = 1;
				layoutBindings[i][k].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				layoutBindings[i][k].pImmutableSamplers = nullptr;
				k++;
			}

			for (size_t j = 0; j < numDescSamplers; j++)
			{
				layoutBindings[i][k].binding = static_cast<uint32_t>(k);
				layoutBindings[i][k].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				layoutBindings[i][k].descriptorCount = 1;
				layoutBindings[i][k].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutBindings[i][k].pImmutableSamplers = nullptr;
				k++;
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

	void DescriptorSet::UpdateBuffer(const uint32_t set, const uint32_t binding, const void* data, const uint64_t dataSize)
	{
		size_t loc = Key(set, binding);
		m_DescriptorBuffers.at(loc)->UploadData(data, dataSize);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_DescriptorBuffers.at(loc)->GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = m_DescriptorBuffers.at(loc)->GetDataSize();

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

	void DescriptorSet::UpdateImage(const uint32_t set, const uint32_t binding)
	{
		size_t loc = Key(set, binding);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.sampler = m_DescriptorImages.at(loc)->GetSampler();
		imageInfo.imageView = m_DescriptorImages.at(loc)->GetImageView();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkWriteDescriptorSet writeDescriptorSet{};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = m_DescriptorSets[set];
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.dstArrayElement = 0;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSet.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(m_Device->GetVkDevice(), 1, &writeDescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t set)
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[set], 0, 0);
	}

	size_t DescriptorSet::Key(const uint32_t i, const uint32_t j) const
	{
		return (size_t)i << 32 | (unsigned int)j;
	}
}
