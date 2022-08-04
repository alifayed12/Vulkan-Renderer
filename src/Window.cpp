#include "Window.hpp"

namespace VE
{
    Window::Window(int width, int height, const std::string& windowTitle)
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