#pragma once

#include "Buffer.hpp"

namespace VE
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(Device* device, uint64_t dataSize);
	public:
		void UploadData(const std::vector<Vertex>& vertices) override;
		void BindBuffer(VkCommandBuffer commandBuffer) override;
	protected:
		void CreateBuffer() override;
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) override;
	};
}

