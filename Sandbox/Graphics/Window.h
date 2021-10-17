#pragma once

#include "Renderer.h"

#include <glad/glad.h>

#include <memory>

struct GLFWwindow;

namespace Core
{
    class Registry;
}

namespace Input
{
    class InputSystem;
}

namespace Graphics
{

    class Window
    {
    public:
        Window( Core::Registry & registry, Input::InputSystem & inInputSystem );
        Window( unsigned int width, unsigned int height, Core::Registry & registry, Input::InputSystem& inInputSystem );
        ~Window();

		Renderer& GetRenderer() noexcept
		{
			return *m_renderer.get();
		}

        void PollInput();
        void PreRender();

        double GetCurrentFrameTime() const;
        double GetDeltaTime() const;

        void Render();

        void ImguiPreRender();
        void ImguiRender();

        bool ShouldClose() const noexcept;

        operator bool() const noexcept
        {
            return m_glfwWindow != nullptr;
        }

        int GetRenderedFrames() const noexcept
        {
            return m_fps;
        }

        static constexpr glm::vec4 Red   { glm::vec4( 1.f, 0.f, 0.f, 1.f ) };
        static constexpr glm::vec4 Green { glm::vec4( 0.f, 1.f, 0.f, 1.f ) };
        static constexpr glm::vec4 Blue  { glm::vec4( 0.f, 0.f, 1.f, 1.f ) };
        static constexpr glm::vec4 Black { glm::vec4( 0.f, 0.f, 0.f, 1.f ) };
        static constexpr glm::vec4 White { glm::vec4( 1.f, 1.f, 1.f, 1.f ) };

        bool DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour = White, float thickness = 1.f ) noexcept;

    private:

        class GLFWCallbackWrapper
        {
        public:
            GLFWCallbackWrapper() = delete;
            GLFWCallbackWrapper( const GLFWCallbackWrapper & ) = delete;
            GLFWCallbackWrapper( GLFWCallbackWrapper && ) = delete;
            ~GLFWCallbackWrapper() = delete;

            static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
			static void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
			static void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
			static void CursorPositionCallback( GLFWwindow* window, double xPosition, double yPosition );
			static void ScrollCallback( GLFWwindow* window, double xOffset, double yOffset );
            static void SetApplication( Window * application );
        private:
            static Window * s_window;
        };

        void CreateWindow();

        void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
		void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
		void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
		void CursorPositionCallback( GLFWwindow* window, double xPosition, double yPosition );
		void ScrollCallback( GLFWwindow* window, double xOffset, double yOffset );

        void ProcessInput();
        void InitImgui();

        Core::Registry & m_registry;
        Input::InputSystem & m_inputSystem;


        GLFWwindow * m_glfwWindow;
        std::unique_ptr<Renderer> m_renderer;

        unsigned int m_screenWidth { 800 };
        unsigned int m_screenHeight { 600 };

        double m_lastFrameTime { 0.f };
        double m_deltaFrameTime { 0.f };
        double m_currentFrameTime { 0.f };

        double m_frameTimer { 0.f };
        int m_renderedFrames { 0 };
        int m_fps { 0 };

    };
}
