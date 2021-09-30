#include "ImguiWindow.h"

#include "glad/glad.h"
#include "Imgui/imgui.h"

#include "Graphics/Window.h"

#include "Core/Game.h"

#include "BasicMovementSubsystem.h"

namespace Game
{
    //ImGuiWindow::ImGuiWindow( entt::registry & registry, Graphics::Window const & windowRef )
    //    : Core::ImGuiWindowBuilder( registry )
    //    , m_windowRef( windowRef )
    //{

    //}

    //ImGuiWindow::~ImGuiWindow()
    //{
    //}

    //void ImGuiWindow::BuildWindow( double deltaTime )
    //{
    //    auto view = m_registry.view<BasicMovementSubsystem::MovementStruct>();
    //    auto entity = view.front();

    //    BasicMovementSubsystem::MovementStruct & MovementStruct = m_registry.get<BasicMovementSubsystem::MovementStruct>( entity );

    //    if( ImGui::BeginMainMenuBar() )
    //    {
    //        if( ImGui::BeginMenu( "Stats" ) )
    //        {
    //            ImGui::Checkbox( "Basic Movement Stats", &m_showWindow );
    //            ImGui::EndMenu();
    //        }
    //        ImGui::EndMainMenuBar();
    //    }

    //    if( m_showWindow )
    //    {
    //        if( ImGui::Begin( "Stats" ) )
    //        {
    //            ImGui::Text( "Basic Movement" );
    //            ImGui::DragFloat2( "Position", &MovementStruct.m_xy.x );
    //            ImGui::DragFloat( "Rotation", &MovementStruct.m_angle, 0.1f );
    //            ImGui::Separator();
    //        }
    //        ImGui::End();
    //    }
    //}



}