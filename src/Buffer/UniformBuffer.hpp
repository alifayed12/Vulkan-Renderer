#pragma once

#include "Buffer.hpp"

namespace VE
{
	class UniformBuffer final : public Buffer
	{
	public:
		UniformBuffer(std::shared_ptr<Device> device, uint64_t dataSize);

		UniformBuffer(const UniformBuffer& otherBuffer) = delete;
		UniformBuffer& operator=(const UniformBuffer& otherBuffer) = delete;
	private:
		void BindBuffer(VkCommandBuffer commandBuffer) const override;
		void CreateBuffer() override;
		uint32_t GetDataCount() const override;
	};
}

