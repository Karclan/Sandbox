#include "Game.h"

#include "Subsystem.h"

#include "../Graphics/Window.h"
#include "../Imgui/imgui.h"
#include "../Utils/LoggingWindow.h"

namespace Core
{
    static constexpr double fixedUpdateRate = 1.f / 60.f;

    Game::Game()
        : m_subsystems()
        , m_mainRegistry()
        , m_window( nullptr )
        , m_baseImguiWindowBuilder( nullptr )
        , m_logWindow( nullptr )
    {
        m_window = std::make_unique<Graphics::Window>( m_mainRegistry );
        m_baseImguiWindowBuilder = std::make_unique<Core::BaseImGuiWindowBuilder>( m_mainRegistry, *m_window.get() );
        m_logWindow = std::make_unique<Utils::LoggingWindow>( m_mainRegistry );
    }

    Game::~Game()
    {
    }

    void Game::Init()
    {
        m_baseImguiWindowBuilder->GetInfo();
    }

    void Game::RegisterSubsystem( std::shared_ptr<Subsystem> subsystem )
    {
        //TODO: Sort based on update order
        m_subsystems.push_back( subsystem );
    }

    void Game::RegisterImguiWindow( std::shared_ptr<ImGuiWindowBuilder> window )
    {
        m_imguiWindows.push_back( window );
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

        accumulator += frameTime;

        int const framesToCatchUpOn = static_cast< int >( accumulator / fixedUpdateRate );

        m_accumulator = accumulator;
        m_framesToCatchUpOn = framesToCatchUpOn;

        while( accumulator >= fixedUpdateRate )
        {
            for( auto & subSystem : m_subsystems )
            {
                subSystem->FixedUpdate( fixedUpdateRate );
            }
            accumulator -= fixedUpdateRate;
        }

        for( auto & subSystem : m_subsystems )
        {
            subSystem->Update( frameTime );
        }

        m_baseImguiWindowBuilder->BuildWindow( frameTime );
        m_logWindow->BuildWindow( frameTime );
		m_window->GetRenderer()->BuildImGui( static_cast<float>( frameTime ) );
		for( auto & imguiWindow : m_imguiWindows )
		{
		    imguiWindow->BuildWindow( frameTime );
		}

        m_window->Render();
    }

    void Game::sortSubsytems()
    {

    }
}