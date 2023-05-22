#pragma once

#include "CoreRegistry.h"

#include "../Editor/Utilities.h"
#include "../Editor/EditorInspector.h"
#include "../Input/InputSystem.h"
#include "../Utils/LoggingWindow.h"
#include "../Graphics/CameraSystem.h"
#include "../Graphics/Window.h"
#include "../Physics/CollisionDetectionSystem.h"

#include <memory>
#include <vector>

namespace Graphics
{
	class Window;
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
		virtual ~Game();
		Game( Game const & other ) = delete;
		Game( Game const && other ) = delete;
		Game & operator=( Game const & other ) = delete;
		Game & operator=( Game const && other ) = delete;

		virtual void Init();

		//Subsystem functions
		virtual void RegisterSubsystem( Subsystem * subsystem );
		virtual void RegisterImguiWindow( ImGuiWindowBuilder * subsystem );
		virtual void DeregisterSubsystem( Subsystem * subsystem ) noexcept;
		virtual void DeregisterImguiWindow( ImGuiWindowBuilder * subsystem ) noexcept;

		virtual bool ShouldClose() const noexcept;
		virtual void Run();

		Core::Registry const & GetRegistry() const noexcept { return m_mainRegistry; }
		Core::Registry & GetRegistry() noexcept { return m_mainRegistry; }

		Graphics::Window const & GetWindow() const noexcept { return m_window; }
		Graphics::Window & GetWindow() noexcept { return m_window; }

		Input::InputSystem & GetInputSystem() noexcept { return m_inputSystem; }
		Input::InputSystem const & GetInputSystem() const noexcept { return m_inputSystem; }
		
		Graphics::CameraSystem & GetCameraSystem() noexcept { return m_cameraSystem; }
		Graphics::CameraSystem const & GetCameraSystem() const noexcept { return m_cameraSystem; }

	private:
		//Sorts subsystem vector based on update order
		virtual void SortSubsytems() noexcept;

		//Fixed update values
		int m_framesToCatchUpOn { 0 };
		double m_accumulator = 0.f;
		double m_fixedUpdateRate = 0.f;

		//Registry
		Core::Registry m_mainRegistry {};

		//Editor
		Editor::Utilities m_editorUtilities {};
		Editor::EditorInspector m_editorInspector;

		//Core Systems
		Input::InputSystem m_inputSystem;
		Utils::LoggingWindow m_logWindow;
		Graphics::CameraSystem m_cameraSystem;
		
		Graphics::Window m_window;

		Physics::CollisionDetectionSystem m_collisionDetectionSystem;

		//std::unique_ptr<Graphics::Window> m_window { nullptr };
		std::unique_ptr<Core::BaseImGuiWindowBuilder> m_baseImguiWindowBuilder { nullptr };

		std::vector<Subsystem *> m_subsystems {};
		std::vector<ImGuiWindowBuilder *> m_imguiWindows {};
	};
}