#pragma once

#include "Buffer.hpp"

namespace VE
{
	class StagingBuffer final : public Buffer
	{
	public:
		StagingBuffer(std::shared_ptr<Device> device, uint64_t dataSize);

		StagingBuffer(const StagingBuffer& otherBuffer) = delete;
		StagingBuffer& operator=(const StagingBuffer& otherBuffer) = delete;
	public:
		void BindBuffer(VkCommandBuffer commandBuffer) const override;
	private:
		virtual void CreateBuffer() override;
		virtual uint32_t GetDataCount() const override;
	};
}

