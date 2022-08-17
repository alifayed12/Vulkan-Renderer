#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Utilities.hpp"

#include <stdexcept>

namespace VE
{
	Texture::Texture(Device* device)
		:	m_Device(device), m_StagingBuffer(nullptr), m_Path(), 
			m_TexWidth(0), m_TexHeight(0), m_TexChannels(0),
			m_Image(VK_NULL_HANDLE), m_ImageView(VK_NULL_HANDLE),
			m_Sampler(VK_NULL_HANDLE), m_ImageMemory(VK_NULL_HANDLE)
	{
	}

	Texture::~Texture()
	{
		if (m_Sampler)
		{
			vkDestroySampler(m_Device->GetVkDevice(), m_Sampler, nullptr);
		}
		if (m_ImageView)
		{
			vkDestroyImageView(m_Device->GetVkDevice(), m_ImageView, nullptr);
		}
		if (m_Image)
		{
			vkDestroyImage(m_Device->GetVkDevice(), m_Image, nullptr);
		}
		if (m_ImageMemory)
		{
			vkFreeMemory(m_Device->GetVkDevice(), m_ImageMemory, nullptr);
		}
	}

	void Texture::Create(std::string_view filePath)
	{
		m_Path = filePath.data();

		LoadImage();
		CreateImageView();
		CreateSampler();
	}

	void Texture::LoadImage()
	{
		stbi_uc* pixels = stbi_load(m_Path.c_str(), &m_TexWidth, &m_TexHeight, &m_TexChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = m_TexWidth * m_TexHeight * 4;

		if (!pixels)
		{
			throw std::runtime_error("Error: Failed to load image from " + m_Path);
		}

		m_StagingBuffer = std::make_unique<StagingBuffer>(m_Device, imageSize);
		m_StagingBuffer->UploadData(pixels, imageSize);

		stbi_image_free(pixels);

		CreateImage();
		TransitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage();
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void Texture::CreateImage()
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(m_TexWidth);
		imageInfo.extent.height = static_cast<uint32_t>(m_TexHeight);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		VK_CHECK(vkCreateImage(m_Device->GetVkDevice(), &imageInfo, nullptr, &m_Image))

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_Device->GetVkDevice(), m_Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(m_Device->GetVkDevice(), &allocInfo, nullptr, &m_ImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(m_Device->GetVkDevice(), m_Image, m_ImageMemory, 0);
	}

	void Texture::TransitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = m_Device->BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("Error: This layout transition is unsupported!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		m_Device->EndSingleTimeCommands(commandBuffer);
	}

	void Texture::CopyBufferToImage()
	{
		VkCommandBuffer commandBuffer = m_Device->BeginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			static_cast<uint32_t>(m_TexWidth),
			static_cast<uint32_t>(m_TexHeight),
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			m_StagingBuffer->GetVkBuffer(),
			m_Image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		m_Device->EndSingleTimeCommands(commandBuffer);
	}

	void Texture::CreateImageView()
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(m_Device->GetVkDevice(), &viewInfo, nullptr, &m_ImageView))
	}

	void Texture::CreateSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_Device->GetPhysicalDevice(), &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VK_CHECK(vkCreateSampler(m_Device->GetVkDevice(), &samplerInfo, nullptr, &m_Sampler))
	}

	uint32_t Texture::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_Device->GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			// typeFilter bitwise and checks if this memory type can be used
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Error: Failed to find suitable memory type!");
	}
}
