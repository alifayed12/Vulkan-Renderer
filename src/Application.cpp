#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "Model.hpp"

#include <iostream>

namespace VE
{
    Application::Application()
        : m_Window(WIDTH, HEIGHT, "Vulkan Engine"),
            m_Device(std::make_shared<Device>(&m_Window))
    {
    }

    void Application::Run()
    {
        const std::vector<Vertex> vertices =
        {
            {{-0.5f, -0.5f},    {0.463f, 0.470f, 0.929f}},
            {{ 0.5f, -0.5f},    {0.463f, 0.470f, 0.929f}},
            {{ 0.5f,  0.5f},    {0.463f, 0.470f, 0.929f}},
            {{-0.5f,  0.5f},    {0.463f, 0.470f, 0.929f}}
        };

        const std::vector<uint16_t> indices = 
        {
            0, 1, 2, 2, 3, 0
        };

        Model rectangle(m_Device, vertices, indices);
        //Model car("ModelPath.file");

        Renderer renderer(&m_Window, m_Device);

        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
            renderer.DrawFrame(rectangle);
        }

        vkDeviceWaitIdle(m_Device->GetVkDevice());
    }
}