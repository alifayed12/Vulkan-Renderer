#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include <iostream>

namespace VE
{
    Application::Application()
        : m_Window(WIDTH, HEIGHT, "Vulkan"),
            m_Device(std::make_shared<Device>(&m_Window)),
            m_Renderer(&m_Window, m_Device)
    {
    }

    void Application::Run()
    {
        const std::vector<Vertex> vertices =
        {
            {{-0.5f, -0.5f},    {0.484f, 0.815f, 0.776f}},
            {{ 0.5f, -0.5f},    {0.484f, 0.815f, 0.776f}},
            {{ 0.5f,  0.5f},    {0.164f, 0.615f, 0.561f}},
            {{-0.5f,  0.5f},    {0.164f, 0.615f, 0.561f}}
        };

        const std::vector<uint16_t> indices = 
        {
            0, 1, 2, 2, 3, 0
        };

        uint64_t vertexDataSize = static_cast<uint64_t>(vertices.size()) * sizeof(Vertex);
        uint64_t indexDataSize = static_cast<uint64_t>(indices.size()) * sizeof(uint16_t);

        VertexBuffer vert(m_Device, vertexDataSize, vertices.data());
        IndexBuffer index(m_Device, indexDataSize, indices.data());

        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
            m_Renderer.DrawFrame(vert, index);
        }

        vkDeviceWaitIdle(m_Device->GetDevice());
    }
}