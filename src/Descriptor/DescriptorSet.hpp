#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"
#include "Texture.hpp"
#include "Buffer/UniformBuffer.hpp"

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
		struct GlobalUniform
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};
	public:
		DescriptorSet(Device* device);
		~DescriptorSet();
	public:
		inline const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }
	public:
		void Create(const std::vector<uint32_t>& descriptorSetInfo);
		void Update(const uint32_t set, const uint32_t binding, const void* data, const uint64_t dataSize);
		void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t set);
	private:
		void CreateDescriptorPool(const uint32_t numUniformDescriptors);
	private:
		Device*												m_Device;
		VkDescriptorPool									m_DescriptorPool;
		std::vector<VkDescriptorSetLayout>					m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet>						m_DescriptorSets;
		std::vector<std::vector<std::unique_ptr<Buffer>>>	m_DescriptorBuffers;
		std::vector<std::vector<std::unique_ptr<Texture>>>	m_DescriptorImages;
	};
}

