#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"

namespace VE
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(std::shared_ptr<Device> device, uint64_t dataSize, const void* data);
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

