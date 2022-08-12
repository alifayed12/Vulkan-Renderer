#pragma once

#include "Window.hpp"
#include "Renderer.hpp"

#include <memory>

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
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;
    private:
        Window  m_Window;
        Device  m_Device;
    };
}
