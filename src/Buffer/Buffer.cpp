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
		if (m_Buffer)
		{
			vkDestroyBuffer(m_Device->GetVkDevice(), m_Buffer, nullptr);
		}
		if (m_DeviceMemory)
		{
			vkFreeMemory(m_Device->GetVkDevice(), m_DeviceMemory, nullptr);
		}
	}

	void Buffer::UploadData(const void* memory, const uint64_t dataSize)
	{
		assert(dataSize == m_DataSize);

		void* data;
		vkMapMemory(this->m_Device->GetVkDevice(), m_DeviceMemory, 0, this->m_DataSize, 0, &data);
		memcpy(data, memory, static_cast<std::size_t>(this->m_DataSize));
		vkUnmapMemory(this->m_Device->GetVkDevice(), m_DeviceMemory);
	}
}
