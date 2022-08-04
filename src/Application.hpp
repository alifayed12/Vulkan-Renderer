#pragma once

#include "Window.hpp"
#include "Renderer.hpp"

namespace VE
{
    class Application
    {
    public:
        Application();
        ~Application() = default;

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
    public:
        void Run();
    public:
        static constexpr int WIDTH = 900;
        static constexpr int HEIGHT = 600;
    private:
        Window m_Window;
        Device m_Device;
        Renderer m_Renderer;
    };
}
