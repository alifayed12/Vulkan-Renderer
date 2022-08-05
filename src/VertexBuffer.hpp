#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"

namespace VE
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(std::shared_ptr<Device> device, uint64_t dataSize, const void* data);
	public:
		void BindBuffer(VkCommandBuffer commandBuffer) override;
		void CopyBuffer(VkBuffer srcBuffer, VkDeviceSize memSize);
	private:
		void CreateBuffer() override;
	private:
		StagingBuffer m_StagingBuffer;
	};
}

