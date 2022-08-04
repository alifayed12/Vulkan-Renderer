#pragma once

#include <vulkan/vulkan.h>

#include "Window.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"
#include "VertexBuffer.hpp"
#include "Mesh.hpp"

namespace VE
{
	class Renderer
	{
	public:
		Renderer(Window* window, Device* device);
		~Renderer() = default;

		Renderer(const Renderer& otherRenderer) = delete;
		Renderer& operator=(const Renderer& otherRenderer) = delete;
	public:
		void DrawFrame(VertexBuffer& buffer);
	private:
		void CreateCommandBuffer();
		void BeginFrame(VkCommandBuffer commandBuffer);
		void EndFrame(VkCommandBuffer commandBuffer);
	private:
		inline VkCommandBuffer GetCurrentCommandBuffer() { return m_CommandBuffers[m_Swapchain.GetCurrentFrame()]; }
	private:
		std::vector<VkCommandBuffer>	m_CommandBuffers;
		Window*			m_Window;
		Device*			m_Device;
		Swapchain		m_Swapchain;
		Pipeline		m_Pipeline;
		uint32_t		m_CurrentImageIndex;
	};
}

