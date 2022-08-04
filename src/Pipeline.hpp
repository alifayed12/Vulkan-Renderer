#pragma once

#include <vulkan/vulkan.h>

#include <string_view>
#include <span>
#include <vector>

#include "Device.hpp"
#include "Swapchain.hpp"

namespace VE
{
    class Pipeline
    {
    public:
        Pipeline(Device* device, Swapchain* swapchain);
        ~Pipeline();

        Pipeline(const Pipeline& otherPipeline) = delete;
        Pipeline& operator=(const Pipeline& otherPipeline) = delete;
    public:
        inline VkPipeline GetGraphicsPipeline() { return m_GraphicsPipeline; }
    private:
        std::vector<char> ReadShaderFile(std::string_view filepath);
        VkShaderModule CreateShaderModule(std::span<char> shaderCode);
        void CreateGraphicsPipeline();
        void Clean();
    private:
        VkPipeline          m_GraphicsPipeline;
        VkPipelineLayout    m_PipelineLayout;
        Device*     m_Device;
        Swapchain*  m_Swapchain;
    };
}
