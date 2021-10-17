#include "Window.h"

#include "../Core/CoreRegistry.h"
#include "../Input/InputSystem.h"
#include "../Utils/UnusedMacro.h"
#include "../Utils/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	[[maybe_unused]] GLenum severity,
	[[maybe_unused]] GLsizei length,
	const char* message,
	[[maybe_unused]] const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::string debugMessage = "--------------- \n";
	debugMessage.append("Debug message (");
	debugMessage.append(std::to_string(id));
	debugMessage.append("): ");
	debugMessage.append(message);
	debugMessage.append("\n");

	debugMessage.append("Source :");
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		debugMessage.append("API\n");
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		debugMessage.append("System\n");
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		debugMessage.append("Compiler\n");
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		debugMessage.append("Third Party\n");
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		debugMessage.append("Application\n");
		break;
	case GL_DEBUG_SOURCE_OTHER:
		debugMessage.append("Other\n");
		break;
	default:
		debugMessage.append("Unknown source\n");
		break;
	}

	debugMessage.append("Type: ");
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		debugMessage.append("Error\n");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debugMessage.append("Deprecated Behaviour\n");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debugMessage.append("Undefined Behaviour\n");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		debugMessage.append("Portability\n");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		debugMessage.append("Performance\n");
		break;
	case GL_DEBUG_TYPE_MARKER:
		debugMessage.append("Marker\n");
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		debugMessage.append("Push Group\n");
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		debugMessage.append("Pop Group\n"); break;
	case GL_DEBUG_TYPE_OTHER:
		debugMessage.append("Other\n");
		break;
	default:
		debugMessage.append("Unknown type\n");
		break;
	}

	debugMessage.append("Severity :");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		debugMessage.append("high\n");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		debugMessage.append("medium\n");
		break;
	case GL_DEBUG_SEVERITY_LOW:
		debugMessage.append("low\n");
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		debugMessage.append("notification\n");
		break;
	default:
		debugMessage.append("Unknown severity\n");
		break;
	}

	Utils::Logger::LogMessage( Utils::LogType::Error, debugMessage.c_str() );
}

namespace Graphics
{
	Window* Window::GLFWCallbackWrapper::s_window = nullptr;

	Window::Window(Core::Registry& registry, Input::InputSystem& inInputSystem)
		: m_registry(registry)
		, m_inputSystem(inInputSystem)
	{
		GLFWCallbackWrapper::SetApplication(this);
		CreateWindow();
		m_renderer = std::make_unique<Renderer>(registry);
		m_renderer->Init();
	}

	Window::Window(unsigned int width, unsigned int height, Core::Registry& registry, Input::InputSystem& inInputSystem)
		: m_registry(registry)
		, m_inputSystem(inInputSystem)
		, m_screenWidth(width)
		, m_screenHeight(height)
	{
		GLFWCallbackWrapper::SetApplication(this);
		CreateWindow();
		m_renderer = std::make_unique<Renderer>(registry);
	}

	Window::~Window()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
	}

	void Window::CreateWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		//glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

		// glfw window creation
		GLFWwindow* window = glfwCreateWindow(m_screenWidth, m_screenHeight, "Sandbox", NULL, NULL);
		// glfw: initialize and configure
		if (!window)
		{
			Utils::Logger::LogMessage(Utils::LogType::Error, "Failed to create GLFW window.\n");
			glfwTerminate();
			m_glfwWindow = nullptr;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, GLFWCallbackWrapper::FramebufferSizeCallback);

		// tell GLFW to capture our mouse
	   // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		glfwSwapInterval(0);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Utils::Logger::LogMessage(Utils::LogType::Error, "Failed to initalize GLAD.\n");
			m_glfwWindow = nullptr;
		}

		// enable OpenGL debug context if context allows for debug context
		int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
			glDebugMessageCallback(glDebugOutput, NULL);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		m_glfwWindow = window;

		InitImgui();

		glEnable(GL_DEPTH_TEST);
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
		if (m_frameTimer >= 1.f)
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
		glfwSwapBuffers(m_glfwWindow);
	}

	bool Window::ShouldClose() const noexcept
	{
		return glfwWindowShouldClose(m_glfwWindow);
	}

	bool Window::DrawLine(glm::vec3 A, glm::vec3 B, glm::vec4 colour, float thickness) noexcept
	{
		return m_renderer->DrawLine(A, B, colour, thickness);
	}

	void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		UNUSED(window);

		m_screenWidth = width;
		m_screenHeight = width;
		glViewport(0, 0, width, height);
		//Update camera
		m_renderer->FramebufferSizeCallback(width, height);
	}

	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		m_inputSystem.KeyAction(key, scancode, static_cast<Input::InputSystem::ActionState>(action), mods);
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		m_inputSystem.MouseAction(button, static_cast<Input::InputSystem::ActionState>(action), mods);
	}

	void Window::CursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition)
	{
		m_inputSystem.UpdateCursorPosition(xPosition, yPosition);
	}

	void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		m_inputSystem.ScrollAction(xOffset, yOffset);
	}

	void Window::ProcessInput()
	{
		if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_glfwWindow, true);
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
		glfwGetFramebufferSize(m_glfwWindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
		ImGuiIO& io = ImGui::GetIO();
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
		ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Window::GLFWCallbackWrapper::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		s_window->FramebufferSizeCallback(window, width, height);
	}

	void Window::GLFWCallbackWrapper::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		s_window->KeyCallback(window, key, scancode, action, mods);
	}

	void Window::GLFWCallbackWrapper::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		s_window->MouseButtonCallback(window, button, action, mods);
	}

	void Window::GLFWCallbackWrapper::CursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition)
	{
		s_window->CursorPositionCallback(window, xPosition, yPosition);
	}

	void Window::GLFWCallbackWrapper::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		s_window->ScrollCallback(window, xOffset, yOffset);
	}

	void Window::GLFWCallbackWrapper::SetApplication(Window* window)
	{
		s_window = window;
	}

}
