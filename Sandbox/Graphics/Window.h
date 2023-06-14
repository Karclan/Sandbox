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
		Window( unsigned int width, unsigned int height, Core::Registry & registry, Input::InputSystem & inInputSystem );
		~Window();

		Renderer & GetRenderer() noexcept
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

		static constexpr glm::vec4 Red { 1.f, 0.f, 0.f, 1.f };
		static constexpr glm::vec4 Green { 0.f, 1.f, 0.f, 1.f };
		static constexpr glm::vec4 Blue { 0.f, 0.f, 1.f, 1.f };
		static constexpr glm::vec4 Black { 0.f, 0.f, 0.f, 1.f };
		static constexpr glm::vec4 White { 1.f, 1.f, 1.f, 1.f };

		bool DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour = White, float thickness = 1.f ) noexcept;

	private:

		class GLFWCallbackWrapper
		{
		public:
			GLFWCallbackWrapper() = delete;
			GLFWCallbackWrapper( const GLFWCallbackWrapper & ) = delete;
			GLFWCallbackWrapper( GLFWCallbackWrapper && ) = delete;
			~GLFWCallbackWrapper() = delete;

			static void WindowPositionCallback( GLFWwindow * window, int xPosition, int yPosition );
			static void WindowSizeCallback( GLFWwindow * window, int width, int height );
			static void WindowCloseCallback( GLFWwindow * window );
			static void WindowRefreshCabllack( GLFWwindow * window );
			static void WindowFocusCallback( GLFWwindow * window, int focused );
			static void WindowIconifyCallback( GLFWwindow * window, int iconified );
			static void WindowMaximizedCallback( GLFWwindow * window, int maximized );
			static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
			static void WindowContentScaleCallback( GLFWwindow * window, float xScale, float yScale );

			static void KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods );
			static void MouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
			static void CursorPositionCallback( GLFWwindow * window, double xPosition, double yPosition );
			static void ScrollCallback( GLFWwindow * window, double xOffset, double yOffset );

			static void SetApplication( Window * application );
		private:
			static Window * s_window;
		};

		void CreateWindow();

		void WindowPositionCallback( GLFWwindow * window, int xPosition, int yPosition );
		void WindowSizeCallback( GLFWwindow * window, int width, int height );
		void WindowCloseCallback( GLFWwindow * window );
		void WindowRefreshCabllack( GLFWwindow * window );
		void WindowFocusCallback( GLFWwindow * window, int focused );
		void WindowIconifyCallback( GLFWwindow * window, int iconified );
		void WindowMaximizedCallback( GLFWwindow * window, int maximized );
		void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
		void WindowContentScaleCallback( GLFWwindow * window, float xScale, float yScale );

		void KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods );
		void MouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
		void CursorPositionCallback( GLFWwindow * window, double xPosition, double yPosition );
		void ScrollCallback( GLFWwindow * window, double xOffset, double yOffset );

		void ProcessInput();
		void InitImgui();

		Core::Registry & m_registry;
		Input::InputSystem & m_inputSystem;

		GLFWwindow * m_glfwWindow;
		std::unique_ptr<Renderer> m_renderer;

		glm::ivec2 m_windowPosition { 0 };
		bool m_windowPositionChanged { false };
		bool m_windowSizeChanged { false };
		bool m_frameBufferSizeChanged { false };

		unsigned int m_screenWidth { 1920 };
		unsigned int m_screenHeight { 1080 };

		double m_lastFrameTime { 0.f };
		double m_deltaFrameTime { 0.f };
		double m_currentFrameTime { 0.f };

		double m_frameTimer { 0.f };
		int m_renderedFrames { 0 };
		int m_fps { 0 };

	};
}
