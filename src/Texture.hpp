#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"
#include "Buffer/StagingBuffer.hpp"

#include <string>
#include <memory>

namespace VE
{
	class Texture
	{
	public:
		Texture(Device* device, std::string_view filePath);
		~Texture();

		Texture(const Texture& otherTexture) = delete;
		Texture& operator=(const Texture& otherTexture) = delete;
	public:
		inline VkImageView GetImageView() const { return m_ImageView; }
		inline VkSampler GetSampler() const { return m_Sampler; }
	private:
		void LoadImage();
		void CreateImage();
		void TransitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage();
		void CreateImageView();
		void CreateSampler();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	private:
		Device*							m_Device;
		std::unique_ptr<StagingBuffer>	m_StagingBuffer;
		std::string						m_Path;
		int								m_TexWidth;
		int								m_TexHeight;
		int								m_TexChannels;
		VkImage							m_Image;
		VkImageView						m_ImageView;
		VkSampler						m_Sampler;
		VkDeviceMemory					m_ImageMemory;
	};
}

