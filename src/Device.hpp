#pragma once

#include <vulkan/vulkan.h>

#include "Window.hpp"

#include <vector>
#include <optional>
#include <memory>

namespace VE
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        inline bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    class Device
    {
    public:
        explicit Device(Window* window);
        ~Device();

        Device(const Device& otherDevice) = delete;
        Device& operator=(const Device& otherDevice) = delete;
        Device(Device&&) = delete;
        Device& operator=(Device&& otherDevice) = delete;
    public:
        static uint32_t FindMemoryType(Device* device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
        static inline constexpr uint32_t NUM_UNIFORMS = 1;
        static inline constexpr uint32_t NUM_SAMPLERS = 1;
    public:
        inline VkDevice GetVkDevice() const { return m_LogicalDevice; }
        inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        inline VkSurfaceKHR GetSurface() const { return m_Surface; }
        inline SwapchainSupportDetails GetSwapchainSupport() const { return QuerySwapchainSupport(m_PhysicalDevice); }
        inline QueueFamilyIndices GetQueueFamilyIndices() const { return FindQueueFamilies(m_PhysicalDevice); }
        inline VkCommandPool GetCommandPool() const { return m_CommandPool; }
        inline VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        inline VkQueue GetPresentQueue() const { return m_PresentQueue; }
        inline const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }
        inline VkDescriptorPool GetDescriptorPool() const { return m_DescriptorPool; }
    public:
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
        void CreateDescriptorLayouts();
        void CreateDescriptorPool(const uint32_t numObjects);
    private:
        void CreateInstance();
        bool CheckValidationLayers(const std::vector<const char*>& layers);
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
        void CreateLogicalDevice();
        void CreateSurface();
        void CreateCommandPool();
        SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physicalDevice) const;
        void Clean();
    private:
        Window*                             m_Window;
        VkInstance                          m_Instance;
        VkPhysicalDevice                    m_PhysicalDevice;
        VkDevice                            m_LogicalDevice;
        VkQueue                             m_GraphicsQueue;
        VkQueue                             m_PresentQueue;
        VkSurfaceKHR                        m_Surface;
        VkCommandPool                       m_CommandPool;
        std::vector<VkDescriptorSetLayout>  m_DescriptorSetLayouts;
        VkDescriptorPool                    m_DescriptorPool;
    private:
        std::vector<const char*> m_ValidationLayers;
        std::vector<const char*> m_DeviceExtensions;
    };
}