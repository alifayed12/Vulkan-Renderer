#pragma once

#include "Buffer.hpp"

namespace VE
{
	class StagingBuffer : public Buffer
	{
	public:
		StagingBuffer(Device* device, uint64_t dataSize);
	public:
		void BindBuffer(VkCommandBuffer commandBuffer) override;
	private:
		virtual void CreateBuffer() override;
	};
}

