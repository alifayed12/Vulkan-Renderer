#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "Mesh.hpp"

namespace VE
{
    Application::Application()
        : m_Window(WIDTH, HEIGHT, "Vulkan"), m_Device(&m_Window),
        m_Renderer(&m_Window, &m_Device)
    {
    }

    void Application::Run()
    {
        std::vector<Vertex> vertices =
        {
            {{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}
        };

        VertexBuffer vert(&m_Device, static_cast<uint64_t>(vertices.size()) * sizeof(Vertex));
        vert.UploadData(vertices);

        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
            m_Renderer.DrawFrame(vert);
        }

        vkDeviceWaitIdle(m_Device.GetDevice());
    }
}