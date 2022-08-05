#include "Buffer.hpp"

namespace VE
{
	Buffer::Buffer(Device* device, uint64_t dataSize)
		: m_Device(device), m_Buffer(VK_NULL_HANDLE),
			m_DeviceMemory(VK_NULL_HANDLE), m_DataSize(dataSize)
	{
	}

	Buffer::~Buffer()
	{
		//vkDeviceWaitIdle(m_Device->GetDevice());
		if (m_Buffer)
		{
			vkDestroyBuffer(m_Device->GetDevice(), m_Buffer, nullptr);
		}
		if (m_DeviceMemory)
		{
			vkFreeMemory(m_Device->GetDevice(), m_DeviceMemory, nullptr);
		}
	}

	uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

	void Buffer::UploadData(const void* memory)
	{
		void* data;
		vkMapMemory(this->m_Device->GetDevice(), m_DeviceMemory, 0, this->m_DataSize, 0, &data);
		memcpy(data, memory, static_cast<std::size_t>(this->m_DataSize));
		vkUnmapMemory(this->m_Device->GetDevice(), m_DeviceMemory);
	}
}
