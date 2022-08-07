#include "VertexBuffer.hpp"

namespace VE
{
	VertexBuffer::VertexBuffer(std::shared_ptr<Device> device, uint64_t dataSize, const void* data)
		: Buffer(device, dataSize), m_StagingBuffer(device, dataSize)
	{
		CreateBuffer();
		m_StagingBuffer.UploadData(data);
		CopyBuffer(m_StagingBuffer.GetVKBuffer(), dataSize);
	}

	void VertexBuffer::CreateBuffer()
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = this->m_DataSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(this->m_Device->GetDevice(), &bufferInfo, nullptr, &this->m_Buffer))

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(this->m_Device->GetDevice(), this->m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VK_CHECK(vkAllocateMemory(this->m_Device->GetDevice(), &allocInfo, nullptr, &m_DeviceMemory))
		vkBindBufferMemory(this->m_Device->GetDevice(), this->m_Buffer, m_DeviceMemory, 0);
	}

	void VertexBuffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &this->m_Buffer, offsets);
	}

	void VertexBuffer::CopyBuffer(VkBuffer srcBuffer, VkDeviceSize memSize)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = this->m_Device->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VK_CHECK(vkAllocateCommandBuffers(this->m_Device->GetDevice(), &allocInfo, &commandBuffer))

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = memSize;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, this->m_Buffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(this->m_Device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(this->m_Device->GetGraphicsQueue());

		vkFreeCommandBuffers(this->m_Device->GetDevice(), this->m_Device->GetCommandPool(), 1, &commandBuffer);
	}

	uint32_t VertexBuffer::GetDataCount() const
	{
		return static_cast<uint32_t>(m_DataSize / sizeof(Vertex));
	}
}
