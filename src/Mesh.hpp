#pragma once

#include "VertexBuffer.hpp"

namespace VE
{
	class Mesh
	{
	public:
		Mesh(Device* device, const std::vector<Vertex>& vertices);
		~Mesh() = default;
	public:
		//inline VkBuffer GetBuffer() const { return m_VertexBuffer.GetBuffer(); }
	private:
		VertexBuffer	m_VertexBuffer;
	};
}

