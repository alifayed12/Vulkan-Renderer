#pragma once

#include <vulkan/vulkan.h>

#include <string_view>
#include <span>
#include <vector>

#include "Device.hpp"
#include "Swapchain.hpp"

namespace VE
{
    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription>    bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription>  attributeDescriptions{};
        VkPipelineViewportStateCreateInfo               viewportInfo{};
        VkPipelineInputAssemblyStateCreateInfo          inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo          rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo            multisampleInfo{};
        VkPipelineColorBlendAttachmentState             colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo             colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo           depthStencilInfo{};
        std::vector<VkDynamicState>                     dynamicStateEnables{};
        VkPipelineDynamicStateCreateInfo                dynamicStateInfo{};
        VkPipelineLayout                                pipelineLayout{};
        VkRenderPass                                    renderPass{};
        uint32_t                                        subpass{};
    };

    class Pipeline
    {
    public:
        Pipeline(std::shared_ptr<Device> device, const PipelineConfigInfo& configInfo);
        ~Pipeline();

        Pipeline(const Pipeline& otherPipeline) = delete;
        Pipeline& operator=(const Pipeline& otherPipeline) = delete;
    public:
        inline VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }
        static void DefaultPipelineConfig(PipelineConfigInfo& configInfo);
    private:
        std::vector<char> ReadShaderFile(std::string_view filepath) const;
        VkShaderModule CreateShaderModule(std::span<char> shaderCode) const;
        void CreateGraphicsPipeline(const PipelineConfigInfo& configInfo);
        void Clean();
    private:
        std::shared_ptr<Device>             m_Device;
        VkPipeline                          m_GraphicsPipeline;
    };
}
