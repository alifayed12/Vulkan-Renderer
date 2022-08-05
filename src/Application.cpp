#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "VertexBuffer.hpp"

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
        std::vector<Vertex> vertices =
        {
            {{0.0f, -0.5f}, {0.484f, 0.815f, 0.776f}},
            {{0.5f, 0.5f},  {0.164f, 0.615f, 0.561f}},
            {{-0.5f, 0.5f}, {0.164f, 0.615f, 0.561f}}
        };

        uint64_t dataSize = static_cast<uint64_t>(vertices.size()) * sizeof(Vertex);
        VertexBuffer vert(m_Device, dataSize, vertices.data());

        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
            m_Renderer.DrawFrame(vert);
        }

        vkDeviceWaitIdle(m_Device->GetDevice());
    }
}