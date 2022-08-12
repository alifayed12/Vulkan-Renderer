#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"

#include <vector>
#include <memory>

namespace VE
{
    class Swapchain
    {
    public:
        Swapchain(Device* device, Window* window);
        ~Swapchain();

        Swapchain(const Swapchain& otherSwapchain) = delete;
        Swapchain& operator=(const Swapchain& otherSwapchain) = delete;
    public:
        static inline constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
    public:
        VkResult AcquireNextImage(uint32_t* currentImageIndex);
        void RecreateSwapchain();
    public:
        inline std::vector<VkImageView> GetImageViews() const { return m_ImageViews; }
        inline VkFormat GetFormat() const { return m_ImageFormat; }
        inline VkExtent2D GetExtent() const { return m_ImageExtent; }
        inline VkRenderPass GetRenderPass() const { return m_RenderPass; }
        inline std::vector<VkFramebuffer> GetFramebuffers() const { return m_Framebuffers; }
        inline std::vector<VkSemaphore> GetImageAvailableSemaphores() const { return m_ImageAvailableSemaphores; }
        inline std::vector<VkSemaphore> GetRenderFinishedSemaphores() const { return m_RenderFinishedSemaphores; }
        inline std::vector<VkFence> GetInFlightFences() const { return m_InFlightFences; }
        inline VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
        inline uint32_t GetCurrentFrame() const { return m_CurrentFrame; }
        inline void SetCurrentFrame(uint32_t value) { m_CurrentFrame = value; }
    private:
        VkSurfaceFormatKHR ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
        void CreateSwapchain();
        void CreateImageViews();
        void CreateSyncObjects();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CleanSwapchain();
        void Clean();
    private:
        VkSwapchainKHR              m_Swapchain;
        Device*                     m_Device;
        Window*                     m_Window;
        VkFormat                    m_ImageFormat;
        VkExtent2D                  m_ImageExtent;
        VkRenderPass                m_RenderPass;
        std::vector<VkFramebuffer>  m_Framebuffers;
        std::vector<VkImage>        m_Images;
        std::vector<VkImageView>    m_ImageViews;
        std::vector<VkSemaphore>    m_ImageAvailableSemaphores;
        std::vector<VkSemaphore>    m_RenderFinishedSemaphores;
        std::vector<VkFence>        m_InFlightFences;
        uint32_t                    m_CurrentFrame;
    };
}
