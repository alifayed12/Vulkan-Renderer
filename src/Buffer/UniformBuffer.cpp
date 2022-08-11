#include "UniformBuffer.hpp"

namespace VE
{
	UniformBuffer::UniformBuffer(std::shared_ptr<Device> device, uint64_t dataSize)
		: Buffer(device, dataSize)
	{
		CreateBuffer();
	}

	void UniformBuffer::CreateBuffer()
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = this->m_DataSize;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(this->m_Device->GetVkDevice(), &bufferInfo, nullptr, &this->m_Buffer))

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(this->m_Device->GetVkDevice(), this->m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK(vkAllocateMemory(this->m_Device->GetVkDevice(), &allocInfo, nullptr, &this->m_DeviceMemory))
		vkBindBufferMemory(this->m_Device->GetVkDevice(), this->m_Buffer, this->m_DeviceMemory, 0);
	}

	void UniformBuffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
	}

	uint32_t UniformBuffer::GetDataCount() const
	{
		return 1;
	}
}