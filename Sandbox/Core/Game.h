#pragma once

#include "CoreRegistry.h"

#include <memory>
#include <vector>

namespace Graphics
{
	class Window;
	class CameraSystem;
}

namespace Utils
{
	class LoggingWindow;
}

namespace Input
{
	class InputSystem;
}

namespace Core
{
	class Subsystem;

	class ImGuiWindowBuilder;
	class BaseImGuiWindowBuilder;

	//Main game class
	//Used to hold the systems together in a useful container
	//Things will probably live in a derived game
	class Game
	{
	public:
		Game();
		~Game();

		virtual void Init();
		virtual void RegisterSubsystem( std::shared_ptr<Subsystem> subsystem );
		virtual void RegisterImguiWindow( std::shared_ptr<ImGuiWindowBuilder> subsystem );
		virtual bool ShouldClose();
		virtual void Run();

		Core::Registry& GetRegistry() { return m_mainRegistry; }
		Graphics::Window* GetWindow() { return m_window.get(); }

	protected:

	private:

		int m_framesToCatchUpOn { 0 };
		double m_accumulator = 0.f;
		double m_fixedUpdateRate = 0.f;

		double accumulator = 0.0f;

		//Sorts subsystem vector based on update order
		virtual void sortSubsytems();

		Core::Registry m_mainRegistry {};
		entt::entity UpdateStatsEntity;

		std::unique_ptr<Input::InputSystem> m_inputSystem;

		std::unique_ptr<Graphics::Window> m_window;

		std::unique_ptr<Core::BaseImGuiWindowBuilder> m_baseImguiWindowBuilder;
		std::unique_ptr<Utils::LoggingWindow> m_logWindow;
		std::unique_ptr<Graphics::CameraSystem> m_cameraSystem;
		std::vector<std::shared_ptr<Subsystem>> m_subsystems;
		std::vector<std::shared_ptr<ImGuiWindowBuilder>> m_imguiWindows;
	};
}