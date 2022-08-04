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
}
