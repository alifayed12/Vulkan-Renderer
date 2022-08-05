#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"

namespace VE
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(Device* device, uint64_t dataSize, StagingBuffer& buffer);
	public:
		void BindBuffer(VkCommandBuffer commandBuffer) override;
		void CopyBuffer(VkBuffer srcBuffer, VkDeviceSize memSize);
	private:
		void CreateBuffer() override;
	};
}

