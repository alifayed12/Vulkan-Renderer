#include "Swapchain.hpp"

#include "Utilities.hpp"

#include <algorithm>
#include <stdexcept>
#include <limits>

namespace VE
{
    Swapchain::Swapchain(Device* device, Window* window)
        : m_Swapchain(VK_NULL_HANDLE), m_Device(device), m_Window(window),
            m_ImageFormat{}, m_ImageExtent{}, m_RenderPass(VK_NULL_HANDLE),
            m_CurrentFrame(0)
    {
        CreateSwapchain();
        CreateImageViews();
        CreateSyncObjects();
        CreateRenderPass();
        CreateFramebuffers();
    }

    Swapchain::~Swapchain()
    {
        Clean();
    }

    VkSurfaceFormatKHR Swapchain::ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
    {
        for(const auto& format : availableFormats)
        {
            if(format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return format;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR Swapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
    {
        for(const auto& presentMode : availablePresentModes)
        {
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return presentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        } else
        {
            int width, height;
            glfwGetFramebufferSize(m_Window->GetGLFWWindow(), &width, &height);

            VkExtent2D actualExtent =
            {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void Swapchain::CreateSwapchain()
    {
        SwapchainSupportDetails details = m_Device->GetSwapchainSupport();

        VkSurfaceFormatKHR format = ChooseSwapchainFormat(details.formats);
        VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
        VkExtent2D extent = ChooseSwapchainExtent(details.capabilities);

        uint32_t imageCount = details.capabilities.minImageCount + 1;

        if(details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        {
            imageCount = details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = m_Device->GetSurface();
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = format.format;
        swapchainCreateInfo.imageColorSpace = format.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = m_Device->GetQueueFamilyIndices();
        uint32_t indicesArr[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
        if(indices.presentFamily.value() != indices.graphicsFamily.value())
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = indicesArr;
        } else
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        swapchainCreateInfo.preTransform = details.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_CHECK(vkCreateSwapchainKHR(m_Device->GetVkDevice(), &swapchainCreateInfo, nullptr, &m_Swapchain))

        uint32_t swapchainImageCount;
        vkGetSwapchainImagesKHR(m_Device->GetVkDevice(), m_Swapchain, &swapchainImageCount, nullptr);
        m_Images.resize(swapchainImageCount);
        vkGetSwapchainImagesKHR(m_Device->GetVkDevice(), m_Swapchain, &swapchainImageCount, m_Images.data());

        m_ImageFormat = format.format;
        m_ImageExtent = extent;
    }

    void Swapchain::CreateImageViews()
    {
        m_ImageViews.resize(m_Images.size());

        for(size_t i = 0; i < m_ImageViews.size(); i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = m_Images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = m_ImageFormat;
            imageViewCreateInfo.components =
                    {
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY
                    };
            imageViewCreateInfo.subresourceRange =
                    {
                            VK_IMAGE_ASPECT_COLOR_BIT,
                            0,
                            1,
                            0,
                            1
                    };
            VK_CHECK(vkCreateImageView(m_Device->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_ImageViews[i]))
        }
    }

    void Swapchain::CreateSyncObjects()
    {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(m_Device->GetVkDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_Device->GetVkDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_Device->GetVkDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Error: Failed to create semaphores!");
            }
        }
    }

    void Swapchain::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_ImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VK_CHECK(vkCreateRenderPass(m_Device->GetVkDevice(), &renderPassInfo, nullptr, &m_RenderPass))
    }

    void Swapchain::CreateFramebuffers()
    {
        m_Framebuffers.resize(m_ImageViews.size());

        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                m_ImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_ImageExtent.width;
            framebufferInfo.height = m_ImageExtent.height;
            framebufferInfo.layers = 1;

            VK_CHECK(vkCreateFramebuffer(m_Device->GetVkDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]))
        }
    }

    void Swapchain::RecreateSwapchain()
    {
        vkDeviceWaitIdle(m_Device->GetVkDevice());

        CleanSwapchain();

        CreateSwapchain();
        CreateImageViews();
        CreateFramebuffers();
    }

    VkResult Swapchain::AcquireNextImage(uint32_t* currentImageIndex)
    {
        vkWaitForFences(m_Device->GetVkDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        VkResult result = vkAcquireNextImageKHR(m_Device->GetVkDevice(), m_Swapchain, std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, currentImageIndex);

        return result;
    }

    void Swapchain::CleanSwapchain()
    {
        if (m_Framebuffers.size() > 0)
        {
            for (VkFramebuffer framebuffer : m_Framebuffers)
            {
                vkDestroyFramebuffer(m_Device->GetVkDevice(), framebuffer, nullptr);
            }
        }
        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            vkDestroyImageView(m_Device->GetVkDevice(), m_ImageViews[i], nullptr);
        }
        if (m_Swapchain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(m_Device->GetVkDevice(), m_Swapchain, nullptr);
        }
    }

    void Swapchain::Clean()
    {
        CleanSwapchain();

        if (m_ImageAvailableSemaphores.size() > 0)
        {
            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                vkDestroySemaphore(m_Device->GetVkDevice(), m_ImageAvailableSemaphores[i], nullptr);
            }
        }
        if (m_RenderFinishedSemaphores.size() > 0)
        {
            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                vkDestroySemaphore(m_Device->GetVkDevice(), m_RenderFinishedSemaphores[i], nullptr);
            }
        }
        if (m_InFlightFences.size() > 0)
        {
            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                vkDestroyFence(m_Device->GetVkDevice(), m_InFlightFences[i], nullptr);
            }
        }
        if (m_RenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(m_Device->GetVkDevice(), m_RenderPass, nullptr);
        }
    }
}