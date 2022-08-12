#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"

namespace VE
{
	class VertexBuffer final : public Buffer
	{
	public:
		VertexBuffer(Device* device, uint64_t dataSize, const void* data);

		VertexBuffer(const VertexBuffer& otherBuffer) = delete;
		VertexBuffer& operator=(const VertexBuffer& otherBuffer) = delete;
	public:
		void BindBuffer(VkCommandBuffer commandBuffer) const override;
		void CopyBuffer(VkBuffer srcBuffer, VkDeviceSize memSize);
		uint32_t GetDataCount() const override;
	private:
		void CreateBuffer() override;
	private:
		StagingBuffer m_StagingBuffer;
	};
}

