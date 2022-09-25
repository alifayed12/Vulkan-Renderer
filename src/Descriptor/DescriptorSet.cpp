#include "DescriptorSet.hpp"

#include "Buffer/UniformBuffer.hpp"

#include "Swapchain.hpp"

#include "Utilities.hpp"

#include <vector>

namespace VE
{
	DescriptorSet::DescriptorSet(Device* device)
		:	m_Device(device)
	{
	}

	DescriptorSet::~DescriptorSet()
	{
		if (!m_DescriptorSets.empty())
		{
			vkFreeDescriptorSets(m_Device->GetVkDevice(), m_Device->GetDescriptorPool(), m_DescriptorSets.size(), m_DescriptorSets.data());
		}
	}

	void DescriptorSet::Create()
	{
		for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
		{
			size_t k = 0;
			for (size_t j = 0; j < Device::NUM_UNIFORMS; j++)
			{
				m_DescriptorBuffers.insert({ Key((uint32_t)i, (uint32_t)k), std::make_unique<UniformBuffer>(m_Device, sizeof(GlobalUniform)) });
				k++;
			}
			for (size_t j = 0; j < Device::NUM_SAMPLERS; j++)
			{
				m_DescriptorImages.insert({ Key((uint32_t)i, (uint32_t)k), std::make_unique<Texture>(m_Device) });
				k++;
			}
		}

		m_DescriptorSets.resize(m_Device->GetDescriptorSetLayouts().size());

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.descriptorPool = m_Device->GetDescriptorPool();
		descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(m_DescriptorSets.size());
		descriptorSetAllocInfo.pSetLayouts = m_Device->GetDescriptorSetLayouts().data();

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

	void DescriptorSet::SetTexture(const uint32_t set, const uint32_t binding, std::string_view filePath)
	{
		size_t loc = Key(set, binding);
		m_DescriptorImages.at(loc)->Create(filePath);
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
