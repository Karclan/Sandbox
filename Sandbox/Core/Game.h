#pragma once

#include <memory>
#include <vector>

#include <entt/entt.hpp>


namespace Graphics
{
    class Window;
}

namespace Utils
{
    class LoggingWindow;
}

namespace Core
{
    class Subsystem;

    class ImGuiWindowBuilder;
    class BaseImGuiWindowBuilder;

    //Main game class
    //Used to hold the systems together in a useful container
    //Things will probably live in a dervived game
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

        entt::registry& GetRegistry(){ return m_mainRegistry; }
        Graphics::Window* GetWindow(){ return m_window.get(); }

    protected:

    private:

        int m_framesToCatchUpOn { 0 };
        double m_accumulator = 0.f;
        double m_fixedUpdateRate = 0.f;

        double accumulator = 0.0f;

        //Sorts subsystem vector based on update order
        virtual void sortSubsytems();

        entt::registry m_mainRegistry;
        entt::entity UpdateStatsEntity;

        std::unique_ptr<Graphics::Window> m_window;

        std::unique_ptr<Core::BaseImGuiWindowBuilder> m_baseImguiWindowBuilder;
        std::unique_ptr<Utils::LoggingWindow> m_logWindow;
        std::vector<std::shared_ptr<Subsystem>> m_subsystems;
        std::vector<std::shared_ptr<ImGuiWindowBuilder>> m_imguiWindows;
    };
}