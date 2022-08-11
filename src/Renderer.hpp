#pragma once

#include <vulkan/vulkan.h>

#include "Window.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"
#include "Model.hpp"

#include "Descriptor/DescriptorSets.hpp"
#include "Descriptor/DescriptorPool.hpp"

#include <vector>
#include <memory>

namespace VE
{
	class Renderer
	{
	public:
		Renderer(Window* window, std::shared_ptr<Device> device);
		~Renderer();

		Renderer(const Renderer& otherRenderer) = delete;
		Renderer& operator=(const Renderer& otherRenderer) = delete;
	public:
		void DrawFrame(const Model& model);
	private:
		void CreateCommandBuffers();
		void CreatePipelineLayout();
		void CreatePipeline();
		void BeginFrame(VkCommandBuffer commandBuffer);
		void EndFrame(VkCommandBuffer commandBuffer);
	private:
		inline VkCommandBuffer GetCurrentCommandBuffer() const { return m_CommandBuffers[m_Swapchain->GetCurrentFrame()]; }
	private:
		std::vector<VkCommandBuffer>	m_CommandBuffers;
		Window*							m_Window;
		std::shared_ptr<Device>			m_Device;
		std::shared_ptr<Swapchain>		m_Swapchain;
		VkPipelineLayout				m_PipelineLayout;
		std::unique_ptr<Pipeline>		m_Pipeline;
		uint32_t						m_CurrentImageIndex;
	};
}

