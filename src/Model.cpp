#include "Model.hpp"

namespace VE
{
	Model::Model(Device* device, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
		:	m_VertexBuffer(device, vertices.size() * sizeof(Vertex), vertices.data()),
			m_IndexBuffer(device, indices.size() * sizeof(uint16_t), indices.data()),
			m_Transform(1.0f)
	{
	}

	void Model::Bind(VkCommandBuffer commandBuffer) const
	{
		m_VertexBuffer.BindBuffer(commandBuffer);
		m_IndexBuffer.BindBuffer(commandBuffer);
	}

	void Model::Draw(VkCommandBuffer commandBuffer) const
	{
		vkCmdDrawIndexed(commandBuffer, m_IndexBuffer.GetDataCount(), 1, 0, 0, 0);
	}
}
