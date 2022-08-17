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

	using BufferMap = std::unordered_map<size_t, std::unique_ptr<Buffer>>;
	using TextureMap = std::unordered_map<size_t, std::unique_ptr<Texture>>;

	class DescriptorSet
	{
	public:
		struct GlobalUniform
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		struct DescriptorSetInfo
		{
			uint32_t numUniforms;
			uint32_t numSamplers;
		};
	public:
		DescriptorSet(Device* device);
		~DescriptorSet();
	public:
		inline const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }
	public:
		void Create(const std::vector<DescriptorSetInfo>& descriptorSetInfo);
		void UpdateBuffer(const uint32_t set, const uint32_t binding, const void* data, const uint64_t dataSize);
		void UpdateImage(const uint32_t set, const uint32_t binding);
		void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t set);
	private:
		void CreateDescriptorPool(const uint32_t numUniformDescriptors, const uint32_t numSamplerDescriptors);
		size_t Key(const uint32_t i, const uint32_t j) const;
	private:
		Device*								m_Device;
		VkDescriptorPool					m_DescriptorPool;
		std::vector<VkDescriptorSetLayout>	m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet>		m_DescriptorSets;
		BufferMap							m_DescriptorBuffers;
		TextureMap							m_DescriptorImages;
	};
}

