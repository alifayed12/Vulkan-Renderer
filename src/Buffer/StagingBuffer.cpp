#include "StagingBuffer.hpp"

namespace VE
{
	StagingBuffer::StagingBuffer(Device* device, uint64_t dataSize)
		: Buffer(device, dataSize)
	{
		CreateBuffer();
	}

	void StagingBuffer::CreateBuffer()
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = this->m_DataSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(m_Device->GetVkDevice(), &bufferInfo, nullptr, &this->m_Buffer))

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device->GetVkDevice(), this->m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Device::FindMemoryType(m_Device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK(vkAllocateMemory(m_Device->GetVkDevice(), &allocInfo, nullptr, &m_DeviceMemory))
		vkBindBufferMemory(m_Device->GetVkDevice(), this->m_Buffer, m_DeviceMemory, 0);
	}

	void StagingBuffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
		// No need to bind
	}

	uint32_t StagingBuffer::GetDataCount() const
	{
		return 1;
	}
}
