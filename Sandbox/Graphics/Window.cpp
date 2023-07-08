#include "Window.h"

#include "RenderStats.h"

#include "../Core/CoreRegistry.h"
#include "../Input/InputSystem.h"
#include "../Input/InputState.h"
#include "../Utils/UnusedMacro.h"
#include "../Utils/Logger.h"
#include "../Utils/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>
#include <unordered_set>

namespace Graphics
{
	Window * Window::GLFWCallbackWrapper::s_window = nullptr;

	Window::Window( Core::Registry & registry, Input::InputSystem & inInputSystem )
		: m_registry( registry )
		, m_inputSystem( inInputSystem )
	{
		GLFWCallbackWrapper::SetApplication( this );
		CreateWindow();
		m_renderer = std::make_unique<Renderer>( registry, m_screenWidth, m_screenHeight );
		m_renderer->Init();
	}

	Window::Window( unsigned int width, unsigned int height, Core::Registry & registry, Input::InputSystem & inInputSystem )
		: m_registry( registry )
		, m_inputSystem( inInputSystem )
		, m_screenWidth( width )
		, m_screenHeight( height )
	{
		GLFWCallbackWrapper::SetApplication( this );
		CreateWindow();
		m_renderer = std::make_unique<Renderer>( registry, m_screenWidth, m_screenHeight );
		m_renderer->Init();
	}

	Window::~Window()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow( m_glfwWindow );
		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
	}

	void Window::CreateWindow()
	{
		glfwInit();
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );
		//glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

		// glfw window creation
		GLFWwindow * window = glfwCreateWindow( m_screenWidth, m_screenHeight, "Sandbox", NULL, NULL );
		// glfw: initialize and configure
		if ( !window )
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, "Failed to create GLFW window.\n" );
			glfwTerminate();
			m_glfwWindow = nullptr;
		}
		glfwMakeContextCurrent( window );

		glfwSetWindowPosCallback( window, GLFWCallbackWrapper::WindowPositionCallback );
		glfwSetWindowSizeCallback( window, GLFWCallbackWrapper::WindowSizeCallback );
		glfwSetWindowCloseCallback( window, GLFWCallbackWrapper::WindowCloseCallback );
		glfwSetWindowRefreshCallback( window, GLFWCallbackWrapper::WindowRefreshCabllack );
		glfwSetWindowFocusCallback( window, GLFWCallbackWrapper::WindowFocusCallback );
		glfwSetWindowIconifyCallback( window, GLFWCallbackWrapper::WindowIconifyCallback );
		glfwSetWindowMaximizeCallback( window, GLFWCallbackWrapper::WindowMaximizedCallback );
		glfwSetFramebufferSizeCallback( window, GLFWCallbackWrapper::FramebufferSizeCallback );
		glfwSetWindowContentScaleCallback( window, GLFWCallbackWrapper::WindowContentScaleCallback );

		glfwSetKeyCallback( window, GLFWCallbackWrapper::KeyCallback );
		glfwSetMouseButtonCallback( window, GLFWCallbackWrapper::MouseButtonCallback );
		glfwSetScrollCallback( window, GLFWCallbackWrapper::ScrollCallback );
		glfwSetCursorPosCallback( window, GLFWCallbackWrapper::CursorPositionCallback );
		// tell GLFW to capture our mouse
	   // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		glfwSwapInterval( 0 );

		// glad: load all OpenGL function pointers
		if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, "Failed to initalize GLAD.\n" );
			m_glfwWindow = nullptr;
		}

		m_glfwWindow = window;

		InitImgui();


	}

	void Window::PollInput()
	{
		//Input
		glfwPollEvents();

		// input
		ProcessInput();
	}

	void Window::PreRender()
	{
		if ( m_windowPositionChanged )
		{
			RenderStats & renderStats = m_registry.ctx().get< RenderStats >();
			renderStats.ScreenStartingPosition = m_windowPosition;
			m_windowPositionChanged = false;
		}

		if ( m_windowSizeChanged || m_frameBufferSizeChanged )
		{
			RenderStats & renderStats = m_registry.ctx().get< RenderStats >();
			renderStats.ScreenDimensions = { m_screenWidth, m_screenHeight };

			glViewport( 0, 0, m_screenWidth, m_screenHeight );
			//Update camera
			if ( m_windowSizeChanged )
			{
				m_renderer->ScreenSizeCallback( m_screenWidth, m_screenHeight );
				m_windowSizeChanged = false;
			}
			if ( m_frameBufferSizeChanged )
			{
				m_renderer->FramebufferSizeCallback( m_screenWidth, m_screenHeight );
				m_frameBufferSizeChanged = false;
			}
		}

		//glFlush();
		ImguiPreRender();

		m_currentFrameTime = glfwGetTime();
		m_deltaFrameTime = m_currentFrameTime - m_lastFrameTime;

		m_frameTimer += m_deltaFrameTime;
		m_renderedFrames++;
		if ( m_frameTimer >= 1.f )
		{
			m_fps = m_renderedFrames;
			m_renderedFrames = 0;
			m_frameTimer = 0.f;
		}

		m_lastFrameTime = m_currentFrameTime;
	}

	double Window::GetCurrentFrameTime() const
	{
		return m_currentFrameTime;
	}

	double Window::GetDeltaTime() const
	{
		return m_deltaFrameTime;
	}

	void Window::Render()
	{
		//Render
		m_renderer->Render();

		ImguiRender();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers( m_glfwWindow );
	}

	bool Window::ShouldClose() const noexcept
	{
		return glfwWindowShouldClose( m_glfwWindow );
	}

	//bool Window::DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour, float thickness ) noexcept
	//{
	//	return m_renderer->DrawLine( A, B, colour, thickness );
	//}

	//////////////////////////////////////////////////////////////////////////
	//Window callbacks
	//////////////////////////////////////////////////////////////////////////

	void Window::WindowPositionCallback( GLFWwindow * window, int xPosition, int yPosition )
	{
		m_windowPosition = { xPosition, yPosition };
		m_windowPositionChanged = true;
	}

	void Window::WindowSizeCallback( GLFWwindow * window, int width, int height )
	{
		m_screenHeight = height;
		m_screenWidth = width;
		m_windowSizeChanged = true;
	}

	void Window::WindowCloseCallback( GLFWwindow * window )
	{
		//Unimplemented
		// 
		//The callback function is called directly after the close flag has been set. 
		//It can be used for example to filter close requests and clear the close flag again 
		//unless certain conditions are met.
	}

	void Window::WindowRefreshCabllack( GLFWwindow * window )
	{
		//Unimplemented
		//
		//The callback function is called when the contents of the window needs to be refreshed.
		//On compositing window systems such as Aero, Compiz or Aqua, 
		//where the window contents are saved off-screen,
		//this callback might only be called when the window or framebuffer is resized.
	}

	void Window::WindowFocusCallback( GLFWwindow * window, int focused )
	{
		//Unimplemented
		//Keep in mind that it can be very disruptive to the user when a window is forced to the top. 
		//For a less disruptive way of getting the user's attention, see attention requests.
		//The callback function receives changes in the input focus state of the window.
		// 
		//if ( focused )
		//{
		//	//Gained input focus
		//}	
		//else
		//{
		//	//Lost input focus
		//}
	}

	void Window::WindowIconifyCallback( GLFWwindow * window, int iconified )
	{
		//Unimplemented
		// 
		//is function sets the iconification callback of the specified window, 
		//which is called when the window is iconified or restored.
		//if ( iconified )
		//{
		//	// The window was iconified
		//}
		//else
		//{
		//	// The window was restored
		//}
	}

	void Window::WindowMaximizedCallback( GLFWwindow * window, int maximized )
	{
		//Unimplemented
		//The callback function receives changes in the maximization state of the window.
		// 
		//if ( maximized )
		//{
		//	// The window was maximized
		//}
		//else
		//{
		//	// The window was restored
		//}
	}

	void Window::FramebufferSizeCallback( GLFWwindow * window, int width, int height )
	{
		UNUSED( window );

		m_screenWidth = width;
		m_screenHeight = height;
		m_frameBufferSizeChanged = true;

	}

	void Window::WindowContentScaleCallback( GLFWwindow * window, float xScale, float yScale )
	{
		//Unimplemented
		/*The content scale is the ratio between the current DPI and the platform's default DPI.
		  This is especially important for text and any UI elements.
		  If the pixel dimensions of your UI scaled by this look appropriate on your machine then it
		  should appear at a reasonable size on other machines regardless of their DPI and scaling settings.
		  This relies on the system DPI and scaling settings being somewhat correct.
		*/
		//set_interface_scale(xscale, yscale);
	}

	//////////////////////////////////////////////////////////////////////////
	//Input callbacks
	//////////////////////////////////////////////////////////////////////////
	void Window::KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
	{
		m_inputSystem.KeyAction( key, scancode, static_cast< Input::ActionState >( action ), mods );
	}

	void Window::MouseButtonCallback( GLFWwindow * window, int button, int action, int mods )
	{
		m_inputSystem.MouseAction( button, static_cast< Input::ActionState >( action ), mods );
	}

	void Window::CursorPositionCallback( GLFWwindow * window, double xPosition, double yPosition )
	{
		m_inputSystem.UpdateCursorPosition( xPosition, yPosition );
	}

	void Window::ScrollCallback( GLFWwindow * window, double xOffset, double yOffset )
	{
		m_inputSystem.ScrollAction( xOffset, yOffset );
	}

	//////////////////////////////////////////////////////////////////////////

	void Window::ProcessInput()
	{
		if ( glfwGetKey( m_glfwWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		{
			glfwSetWindowShouldClose( m_glfwWindow, true );
		}
	}

	void Window::ImguiPreRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Window::ImguiRender()
	{
		//Imgui render
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize( m_glfwWindow, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		ImGui::EndFrame();
		ImGuiIO & io = ImGui::GetIO();
		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			GLFWwindow * backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent( backup_current_context );
		}
	}

	void Window::InitImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle & style = ImGui::GetStyle();
		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			style.WindowRounding = 0.0f;
			style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL( m_glfwWindow, true );
		ImGui_ImplOpenGL3_Init( "#version 330" );
	}


	//////////////////////////////////////////////////////////////////////////
	//Window callbacks
	//////////////////////////////////////////////////////////////////////////

	void Window::GLFWCallbackWrapper::WindowPositionCallback( GLFWwindow * window, int xPosition, int yPosition )
	{
		s_window->WindowPositionCallback( window, xPosition, yPosition );
	}

	void Window::GLFWCallbackWrapper::WindowSizeCallback( GLFWwindow * window, int width, int height )
	{
		s_window->WindowSizeCallback( window, width, height );
	}

	void Window::GLFWCallbackWrapper::WindowCloseCallback( GLFWwindow * window )
	{
		s_window->WindowCloseCallback( window );
	}

	void Window::GLFWCallbackWrapper::WindowRefreshCabllack( GLFWwindow * window )
	{
		s_window->WindowRefreshCabllack( window );
	}

	void Window::GLFWCallbackWrapper::WindowFocusCallback( GLFWwindow * window, int focused )
	{
		s_window->WindowFocusCallback( window, focused );
	}

	void Window::GLFWCallbackWrapper::WindowIconifyCallback( GLFWwindow * window, int iconified )
	{
		s_window->WindowIconifyCallback( window, iconified );
	}

	void Window::GLFWCallbackWrapper::WindowMaximizedCallback( GLFWwindow * window, int maximized )
	{
		s_window->WindowMaximizedCallback( window, maximized );
	}

	void Window::GLFWCallbackWrapper::FramebufferSizeCallback( GLFWwindow * window, int width, int height )
	{
		s_window->FramebufferSizeCallback( window, width, height );
	}

	void Window::GLFWCallbackWrapper::WindowContentScaleCallback( GLFWwindow * window, float xScale, float yScale )
	{
		s_window->WindowContentScaleCallback( window, xScale, yScale );
	}

	//////////////////////////////////////////////////////////////////////////
	//Input Callbacks
	//////////////////////////////////////////////////////////////////////////

	void Window::GLFWCallbackWrapper::KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
	{
		s_window->KeyCallback( window, key, scancode, action, mods );
	}

	void Window::GLFWCallbackWrapper::MouseButtonCallback( GLFWwindow * window, int button, int action, int mods )
	{
		s_window->MouseButtonCallback( window, button, action, mods );
	}

	void Window::GLFWCallbackWrapper::CursorPositionCallback( GLFWwindow * window, double xPosition, double yPosition )
	{
		s_window->CursorPositionCallback( window, xPosition, yPosition );
	}

	void Window::GLFWCallbackWrapper::ScrollCallback( GLFWwindow * window, double xOffset, double yOffset )
	{
		s_window->ScrollCallback( window, xOffset, yOffset );
	}

	void Window::GLFWCallbackWrapper::SetApplication( Window * window )
	{
		s_window = window;
	}

	//////////////////////////////////////////////////////////////////////////

}
