#include "Window.hpp"

namespace VE
{
    Window::Window(uint32_t width, uint32_t height, const std::string& windowTitle)
        : m_Width(width), m_Height(height), m_WindowTitle(windowTitle), m_Window(nullptr)
    {
        InitWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::InitWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);
    }
}