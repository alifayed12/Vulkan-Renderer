#include "Renderer.hpp"

#include "Utilities.hpp"

#include <stdexcept>
#include <cassert>

namespace VE
{
	Renderer::Renderer(Window* window, std::shared_ptr<Device> device)
		: m_Window(window), m_Device(device), m_Swapchain(std::make_shared<Swapchain>(m_Device, m_Window)),
			m_Pipeline(m_Device, m_Swapchain), m_CurrentImageIndex{}
	{
		CreateCommandBuffer();
	}

	void Renderer::CreateCommandBuffer()
	{
		m_CommandBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Device->GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		VK_CHECK(vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, m_CommandBuffers.data()))
	}

	void Renderer::DrawFrame(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
	{
		VkCommandBuffer currCommandBuffer = GetCurrentCommandBuffer();
		BeginFrame(currCommandBuffer);

		// Draw Here
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_Swapchain->GetExtent().width);
		viewport.height = static_cast<float>(m_Swapchain->GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_Swapchain->GetExtent();

		vkCmdBindPipeline(currCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.GetGraphicsPipeline());
		vkCmdSetViewport(currCommandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(currCommandBuffer, 0, 1, &scissor);

		vertexBuffer.BindBuffer(currCommandBuffer);
		indexBuffer.BindBuffer(currCommandBuffer);

		vkCmdDrawIndexed(currCommandBuffer, indexBuffer.GetDataCount(), 1, 0, 0, 0);

		EndFrame(currCommandBuffer);
	}

	void Renderer::BeginFrame(VkCommandBuffer commandBuffer)
	{
		VkResult result = m_Swapchain->AcquireNextImage(&m_CurrentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_Swapchain->RecreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Error: Failed to acquire swap chain image!");
		}

		vkResetFences(m_Device->GetDevice(), 1, &m_Swapchain->GetInFlightFences()[m_Swapchain->GetCurrentFrame()]);
		vkResetCommandBuffer(commandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		VK_CHECK(vkBeginCommandBuffer(m_CommandBuffers[m_Swapchain->GetCurrentFrame()], &beginInfo))

		VkRenderPassBeginInfo renderPassInfo {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Swapchain->GetRenderPass();
		std::vector<VkFramebuffer> buffers = m_Swapchain->GetFramebuffers();
		renderPassInfo.framebuffer = m_Swapchain->GetFramebuffers()[m_CurrentImageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Swapchain->GetExtent();

		VkClearValue clearColor = { {{0.149f, 0.274f, 0.325f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndFrame(VkCommandBuffer commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);
		VK_CHECK(vkEndCommandBuffer(commandBuffer))

		VkSubmitInfo submitInfo {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_Swapchain->GetImageAvailableSemaphores()[m_Swapchain->GetCurrentFrame()] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { m_Swapchain->GetRenderFinishedSemaphores()[m_Swapchain->GetCurrentFrame()] };

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VK_CHECK(vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_Swapchain->GetInFlightFences()[m_Swapchain->GetCurrentFrame()]))

		VkPresentInfoKHR presentInfo {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		VkSwapchainKHR swapchains[] = { m_Swapchain->GetSwapchain() };

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &m_CurrentImageIndex;

		VkResult result = vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			m_Swapchain->RecreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("Error: Failed to present swapchain image!");
		}

		uint32_t nextFrame = (m_Swapchain->GetCurrentFrame() + 1) % Swapchain::MAX_FRAMES_IN_FLIGHT;
		m_Swapchain->SetCurrentFrame(nextFrame);
	}
}
