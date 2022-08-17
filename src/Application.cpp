#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "Texture.hpp"

#include <iostream>

namespace VE
{
    Application::Application()
        :   m_Window(WIDTH, HEIGHT, "Vulkan Engine"),
            m_Device(&m_Window)
    {
    }

    void Application::Run()
    {
        Model model(&m_Device, "D:\\OpenGL Projects\\VulkanEngine\\Res\\Models\\viking_room.obj");

        Renderer renderer(&m_Window, &m_Device);

        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();

            renderer.DrawFrame(model);
        }

        vkDeviceWaitIdle(m_Device.GetVkDevice());
    }
}