#pragma once

#include "glm/glm.hpp"

#include "Device.hpp"

#include "Buffer/Buffer.hpp"
#include "Buffer/VertexBuffer.hpp"
#include "Buffer/IndexBuffer.hpp"

#include <vector>
#include <memory>

namespace VE
{
	class Model
	{
	public:
		Model(Device* device, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
		~Model() = default;

		Model(const Model& otherModel) = delete;
		Model& operator=(const Model& otherModel) = delete;
	public:
		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;
	public:
		inline const glm::mat4& GetModelTransform() const { return m_Transform; }
	private:
		VertexBuffer			m_VertexBuffer;
		IndexBuffer				m_IndexBuffer;
		glm::mat4				m_Transform;
	};
}

