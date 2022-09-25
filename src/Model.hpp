#pragma once

#include "glm/glm.hpp"

#include "Device.hpp"

#include "Buffer/Buffer.hpp"
#include "Buffer/VertexBuffer.hpp"
#include "Buffer/IndexBuffer.hpp"

#include "Descriptor/DescriptorSet.hpp"

#include <vector>
#include <memory>

namespace VE
{
	class Model
	{
	public:
		Model(Device* device, std::string_view modelPath);
		~Model() = default;

		Model(const Model& otherModel) = delete;
		Model& operator=(const Model& otherModel) = delete;
	public:
		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;
		void UpdateDescriptors(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t currentFrame); // Optimize to not update unless resource change
	public:
		inline const glm::mat4& GetModelTransform() const { return m_Transform; }
	private:
		void LoadModel();
	private:
		Device*							m_Device;
		const char*						m_ModelPath;
		std::unique_ptr<VertexBuffer>	m_VertexBuffer;
		std::unique_ptr<IndexBuffer>	m_IndexBuffer;
		DescriptorSet					m_DescriptorSet;
		glm::mat4						m_Transform;

		DescriptorSet::GlobalUniform	m_GUBO;
	};
}

