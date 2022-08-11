#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Device.hpp"
#include "Utilities.hpp"

#include <array>
#include <vector>
#include <memory>

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
		Buffer(std::shared_ptr<Device>, uint64_t dataSize);
		virtual ~Buffer();
	public:
		inline VkBuffer GetVkBuffer() const { return m_Buffer; }
		inline VkDeviceMemory GetVkDeviceMemory() const { return m_DeviceMemory; }
	public:
		virtual void UploadData(const void* memory, const uint64_t dataSize);
		virtual void BindBuffer(VkCommandBuffer commandBuffer) const = 0;
		virtual uint32_t GetDataCount() const = 0;
	protected:
		virtual void CreateBuffer() = 0;
		virtual uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	protected:
		std::shared_ptr<Device>		m_Device;
		VkBuffer					m_Buffer;
		VkDeviceMemory				m_DeviceMemory;
		uint64_t					m_DataSize;
	};
}

