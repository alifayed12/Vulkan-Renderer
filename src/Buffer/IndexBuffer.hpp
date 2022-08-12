#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"

namespace VE
{
	class IndexBuffer final : public Buffer
	{
	public:
		IndexBuffer(Device* device, uint64_t dataSize, const void* data);

		IndexBuffer(const IndexBuffer& otherBuffer) = delete;
		IndexBuffer& operator=(const IndexBuffer& otherBuffer) = delete;
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

