#include "VertexBuffer.hpp"

#include "Utilities.hpp"

#include <stdexcept>

namespace VE
{
	VertexBuffer::VertexBuffer(Device* device, uint64_t dataSize)
		: Buffer(device, dataSize)
	{
		CreateBuffer();
	}

	void VertexBuffer::CreateBuffer()
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = this->m_DataSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(m_Device->GetDevice(), &bufferInfo, nullptr, &this->m_Buffer))

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device->GetDevice(), this->m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK(vkAllocateMemory(m_Device->GetDevice(), &allocInfo, nullptr, &m_DeviceMemory))
	}

	uint32_t VertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_Device->GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			// typeFilter logical and checks if this memory type can be used
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Error: Failed to find suitable memory type!");
	}

	void VertexBuffer::UploadData(const std::vector<Vertex>& vertices)
	{
		vkBindBufferMemory(m_Device->GetDevice(), this->m_Buffer, m_DeviceMemory, 0);

		void* data;
		vkMapMemory(m_Device->GetDevice(), m_DeviceMemory, 0, this->m_DataSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<std::size_t>(this->m_DataSize));
		vkUnmapMemory(m_Device->GetDevice(), m_DeviceMemory);
	}

	void VertexBuffer::BindBuffer(VkCommandBuffer commandBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &this->m_Buffer, offsets);
	}
}
