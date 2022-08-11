#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace VE
{
    class Window
    {
    public:
        Window(int width, int height, const std::string& windowTitle);
        ~Window();

        Window(const Window& otherWindow) = delete;
        Window& operator=(const Window& otherWindow) = delete;
    public:
        void InitWindow();
    public:
        inline bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
        inline GLFWwindow* GetGLFWWindow() const { return m_Window; }
        inline void PollEvents() const { glfwPollEvents(); }
        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
    private:
        int m_Width;
        int m_Height;
        std::string m_WindowTitle;
        GLFWwindow* m_Window;
    };
}