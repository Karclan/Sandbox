#include "Game.h"

#include "Subsystem.h"

#include "../Graphics/Window.h"

#include "imgui.h"

namespace Core
{
	static constexpr double fixedUpdateRate = 1.f / 60.f;

	Game::Game()
		: m_inputSystem( m_mainRegistry )
		, m_logWindow( m_mainRegistry )
		, m_cameraSystem( m_mainRegistry )
	{
		m_window = std::make_unique<Graphics::Window>( m_mainRegistry, m_inputSystem );
		m_baseImguiWindowBuilder = std::make_unique<Core::BaseImGuiWindowBuilder>( m_mainRegistry, *m_window.get() );

		EditorUtilities.CreateEditorCamera( m_mainRegistry );

		RegisterSubsystem( &m_inputSystem );
		RegisterSubsystem( &m_cameraSystem );

		RegisterImguiWindow( &m_inputSystem );
		RegisterImguiWindow( &m_cameraSystem );

		RegisterImguiWindow( &m_logWindow );
		RegisterImguiWindow( &m_window->GetRenderer() );
	}

	Game::~Game()
	{
	}

	void Game::Init()
	{
		m_baseImguiWindowBuilder->GetInfo();
	}

	void Game::RegisterSubsystem( Subsystem* subsystem )
	{
		//TODO: Sort based on update order
		m_subsystems.push_back( subsystem );
	}

	void Game::RegisterImguiWindow( ImGuiWindowBuilder* window )
	{
		m_imguiWindows.push_back( window );
	}

	void Game::DeregisterSubsystem( Subsystem* subsystem )
	{
		for ( auto it = m_subsystems.begin(); it != m_subsystems.end(); ++it )
		{
			if ( *it == subsystem )
			{
				m_subsystems.erase( it );
				break;
			}
		}
	}

	void Game::DeregisterImguiWindow( ImGuiWindowBuilder* subsystem )
	{
		for ( auto it = m_imguiWindows.begin(); it != m_imguiWindows.end(); ++it )
		{
			if ( *it == subsystem )
			{
				m_imguiWindows.erase( it );
				break;
			}
		}
	}

	bool Game::ShouldClose()
	{
		return m_window->ShouldClose();
	}

	void Game::Run()
	{
		//update input
		m_window->PollInput();

		m_window->PreRender();

		double const frameTime = m_window->GetDeltaTime();

		m_baseImguiWindowBuilder->BuildWindow( frameTime );
		for ( auto& imguiWindow : m_imguiWindows )
		{
			imguiWindow->BuildWindow( frameTime );
		}

		m_accumulator += frameTime;

		int const framesToCatchUpOn = static_cast< int >( m_accumulator / fixedUpdateRate );

		m_accumulator = m_accumulator;
		m_framesToCatchUpOn = framesToCatchUpOn;

		while ( m_accumulator >= fixedUpdateRate )
		{
			for ( auto& subSystem : m_subsystems )
			{
				subSystem->FixedUpdate( fixedUpdateRate );
			}
			m_accumulator -= fixedUpdateRate;
		}

		for ( auto& subSystem : m_subsystems )
		{
			subSystem->Update( frameTime );
		}

		m_window->Render();
	}

	void Game::sortSubsytems()
	{

	}
}