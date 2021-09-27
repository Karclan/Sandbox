#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

void APIENTRY glDebugOutput( GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char * message,
    const void * userParam )
{
    if( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch( source )
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch( type )
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch( severity )
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

namespace Graphics
{
    Window * Window::GLFWCallbackWrapper::s_window = nullptr;

    Window::Window( entt::registry & registry )
        :m_registry( registry )
    {
        GLFWCallbackWrapper::SetApplication( this );
        CreateWindow();
        m_renderer = std::make_unique<Renderer>( registry );
        m_renderer->Init();
    }

    Window::Window( unsigned int width, unsigned int height, entt::registry & registry )
        : m_registry( registry )
        , m_screenWidth( width )
        , m_screenHeight( height )
    {
        GLFWCallbackWrapper::SetApplication( this );
        CreateWindow();
        m_renderer = std::make_unique<Renderer>( registry );
    }

    Window::~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
    }

    Renderer * Window::GetRenderer()
    {
        return m_renderer.get();
    }

    void Window::CreateWindow()
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );
        //glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

        // glfw window creation
        GLFWwindow * window = glfwCreateWindow( m_screenWidth, m_screenHeight, "Sandbox", NULL, NULL );
        // glfw: initialize and configure
        if( !window )
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            m_glfwWindow = nullptr;
        }
        glfwMakeContextCurrent( window );
        glfwSetFramebufferSizeCallback( window, GLFWCallbackWrapper::FramebufferSizeCallback );

        // tell GLFW to capture our mouse
       // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
        glfwSwapInterval( 0 );

        // glad: load all OpenGL function pointers
        if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            m_glfwWindow = nullptr;
        }

        // enable OpenGL debug context if context allows for debug context
        int flags; glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
        if( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
        {
            glEnable( GL_DEBUG_OUTPUT );
            glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS ); // makes sure errors are displayed synchronously
            glDebugMessageCallback( glDebugOutput, NULL );
            glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );
        }

        m_glfwWindow = window;

        InitImgui();

        glEnable( GL_DEPTH_TEST );
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
        //glFlush();
        ImguiPreRender();

        m_currentFrameTime = glfwGetTime();
        m_deltaFrameTime = m_currentFrameTime - m_lastFrameTime;

        m_frameTimer += m_deltaFrameTime;
        m_renderedFrames++;
        if( m_frameTimer >= 1.f )
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

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose( m_glfwWindow );
    }

    bool Window::DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour, float thickness )
    {
        return m_renderer->DrawLine( A, B, colour, thickness );
    }

    void Window::FramebufferSizeCallback( GLFWwindow * window, int width, int height )
    {
        m_screenWidth = width;
        m_screenHeight = width;
        glViewport( 0, 0, width, height );
        //Update camera
        m_renderer->FramebufferSizeCallback( width, height );
    }

    void Window::ProcessInput()
    {
        if( glfwGetKey( m_glfwWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
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
        ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
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

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL( m_glfwWindow, true );
        ImGui_ImplOpenGL3_Init( "#version 330" );
    }

    void Window::GLFWCallbackWrapper::FramebufferSizeCallback( GLFWwindow * window, int width, int height )
    {
        s_window->FramebufferSizeCallback( window, width, height );
    }

    void Window::GLFWCallbackWrapper::SetApplication( Window * window )
    {
        s_window = window;
    }

}
