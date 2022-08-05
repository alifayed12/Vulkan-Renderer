#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Device.hpp"
#include "Utilities.hpp"

#include <array>
#include <vector>

namespace VE
{
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 color;
	};

	class Buffer
	{
	public:
		Buffer(Device* device, uint64_t dataSize);
		virtual ~Buffer();
	public:
		inline VkBuffer GetVKBuffer() const { return m_Buffer; }
	public:
		virtual void UploadData(const void* memory);
		virtual void BindBuffer(VkCommandBuffer commandBuffer) = 0;
	protected:
		virtual void CreateBuffer() = 0;
		virtual uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	protected:
		Device*		m_Device;
		VkBuffer	m_Buffer;
		VkDeviceMemory	m_DeviceMemory;
		uint64_t		m_DataSize;
	};
}

