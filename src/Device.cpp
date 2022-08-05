#include "Device.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <array>
#include <unordered_set>
#include <iostream>

namespace VE
{
    Device::Device(Window* window)
        : m_Window(window), m_Instance(VK_NULL_HANDLE), m_PhysicalDevice(VK_NULL_HANDLE),
            m_LogicalDevice(VK_NULL_HANDLE), m_GraphicsQueue(VK_NULL_HANDLE), m_PresentQueue(VK_NULL_HANDLE),
            m_Surface(VK_NULL_HANDLE), m_CommandPool(VK_NULL_HANDLE)
    {
        m_ValidationLayers =
        {
                "VK_LAYER_KHRONOS_validation"
        };

        m_DeviceExtensions =
        {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateCommandPool();
    }

    Device::~Device()
    {
        Clean();
    }

    void Device::CreateInstance()
    {
        VkApplicationInfo info{};
        info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        info.pApplicationName = "Vulkan Application";
        info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        info.pEngineName = "Application";
        info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &info;

        std::vector<const char*> extensions =
        {
                "VK_KHR_get_physical_device_properties2"
        };

        uint32_t glfwExtensionCount{};
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for(size_t i = 0; i < static_cast<uint32_t>(glfwExtensionCount); i++)
        {
            extensions.push_back(glfwExtensions[i]);
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = 0;

        if(CheckValidationLayers(m_ValidationLayers))
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        }

        if(vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Error: Could not create instance!");
        }
    }

    bool Device::CheckValidationLayers(const std::vector<const char*>& layers)
    {
#ifndef NDEBUG
        uint32_t layerCount{};
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : layers)
        {
            bool layerFound = false;
            for(const auto& layerProp : availableLayers)
            {
                if(strcmp(layerProp.layerName, layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound)
            {
                throw std::runtime_error("Error: Layer \"" + std::string(layerName) + " not found!");
            }
        }

        return true;
#else
        return false;
#endif
    }

    void Device::PickPhysicalDevice()
    {
        uint32_t deviceCount{};
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

        if(deviceCount == 0)
        {
            throw std::runtime_error("Error: Could not find GPUs that support vulkan!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        for(const auto device : devices)
        {
            if(IsDeviceSuitable(device))
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        if(m_PhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Error: Failed to find suitable GPU!");
        }
    }

    bool Device::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);
        bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
        bool swapchainAdequate{};

        if(extensionsSupported)
        {
            SwapchainSupportDetails details = QuerySwapchainSupport(physicalDevice);
            swapchainAdequate = !details.formats.empty() && !details.presentModes.empty();
        }

        return queueFamilyIndices.IsComplete() && extensionsSupported && swapchainAdequate;
    }

    bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
    {
        uint32_t extensionCount{};
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

        std::unordered_set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

        for(const auto& ext : extensions)
        {
            requiredExtensions.erase(ext.extensionName);
        }

        return requiredExtensions.empty();
    }

    QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for(size_t i = 0; i < queueFamilies.size(); i++)
        {
            if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = static_cast<uint32_t>(i);
            }

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), m_Surface, &presentSupport);

            if(presentSupport)
            {
                indices.presentFamily = static_cast<uint32_t>(i);
            }

            if(indices.IsComplete())
            {
                break;
            }
        }

        return indices;
    }

    void Device::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::unordered_set<uint32_t> queueFamilies =
        {
                indices.graphicsFamily.value(),
                indices.presentFamily.value()
        };

        float queuePriorities = 1.0f;
        for(size_t i{}; const uint32_t family : queueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = family;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriorities;
            queueCreateInfos.push_back(queueCreateInfo);
            i++;
        }

        VkPhysicalDeviceFeatures deviceFeatures{}; // No features just yet

        uint32_t extensionCount{};
        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, extensions.data());

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

#ifndef NDEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
#endif

        if(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("Error: Failed to create logical device!");
        }

        vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, indices.presentFamily.value(), 0, &m_PresentQueue);
    }

    void Device::CreateSurface()
    {
        if(glfwCreateWindowSurface(m_Instance, m_Window->GetGLFWWindow(), nullptr, &m_Surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Error: Failed to create surface!");
        }
    }

    void Device::CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Error: Failed to create command pool!");
        }
    }

    SwapchainSupportDetails Device::QuerySwapchainSupport(VkPhysicalDevice physicalDevice)
    {
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);
        if(formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr);
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, details.presentModes.data());

        return details;
    }

    void Device::Clean()
    {
        if (m_CommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
        }
        if(m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, nullptr);
        }
        if(m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }
        if(m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, nullptr);
        }
    }
}
