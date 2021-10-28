#include "Subsystem.h"

#include "../Graphics/Window.h"
#include "../Imgui/imgui.h"

namespace Core
{
	BaseImGuiWindowBuilder::BaseImGuiWindowBuilder( Core::Registry & registry, Graphics::Window const & windowRef ) noexcept
		: Subsystem( registry )
		, m_windowRef( windowRef )
	{
	}

	void BaseImGuiWindowBuilder::GetInfo()
	{
		GLubyte const * vendor = glGetString( GL_VENDOR ); // Returns the vendor
		GLubyte const * renderer = glGetString( GL_RENDERER ); // Returns a hint to the model
		m_vendorName = reinterpret_cast< const char * >( vendor );
		m_modelString = reinterpret_cast< const char * >( renderer );
	}

	void BaseImGuiWindowBuilder::BuildWindow( double deltaTime )
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport const * viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos( viewport->Pos );
		ImGui::SetNextWindowSize( viewport->Size );
		ImGui::SetNextWindowViewport( viewport->ID );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if ( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::Begin( "MainWindow", nullptr, window_flags );
		ImGui::PopStyleVar();
		ImGui::PopStyleVar( 2 );

		ImGuiID const dockspace_id = ImGui::GetID( "MyDockSpace" );
		ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );

		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "File" ) )
			{
				ImGui::Text( "Open.." );
				ImGui::Text( "Edit" );
				ImGui::Text( "Save" );
				ImGui::Separator();
				ImGui::MenuItem( "Test" );
				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Edit" ) )
			{
				ImGui::MenuItem( "Test" );
				ImGui::Separator();
				ImGui::MenuItem( "Test" );
				ImGui::MenuItem( "Test" );
				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Stats" ) )
			{
				if ( ImGui::Checkbox( "Graphics Stats", &m_showGraphicsStats ) )
				{
					if ( m_showGraphicsStats )
					{
						//m_pastFrameTimes.empty();
					}
				}

				ImGui::Checkbox( "Update Stats", &m_showUpdateStats );
				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Options" ) )
			{
				ImGui::Checkbox( "Show Demo Window", &m_showDemoWindow );
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if ( m_showGraphicsStats )
		{
			//m_pastFrameTimes[ m_pastFrameIndex ] = deltaTime;
			//m_pastFrameIndex = ( m_pastFrameIndex + 1 ) % m_frameTimeSize;
			if ( ImGui::Begin( "Stats" ) )
			{
				ImGui::Text( "Model: %s", m_modelString.c_str() );
				ImGui::Text( "vendor: %s", m_vendorName.c_str() );
				ImGui::Separator();
				ImGui::Text( "FrameTime: %f", deltaTime );
				//ImGui::PlotLines( "Frame Times", m_pastFrameTimes.data(), m_frameTimeSize, m_pastFrameIndex);
				//ImGui::PlotHistogram( "Rendered Frames", m_pastFrameTimes.data(), m_frameTimeSize, m_pastFrameIndex );
				ImGui::Separator();
				ImGui::Text( "FPS: %d", m_windowRef.GetRenderedFrames() );
				ImGui::Text( "Imgui FPS: %f", ImGui::GetIO().Framerate );
				ImGui::Separator();
			}
			ImGui::End();
		}

		if ( m_showUpdateStats )
		{
			if ( ImGui::Begin( "Stats" ) )
			{
				ImGui::Text( "CurrentTime: %f", m_windowRef.GetCurrentFrameTime() );
				ImGui::Text( "FrameTime: %f", m_windowRef.GetDeltaTime() );
				ImGui::Text( "Fixed Update Rate: %f", m_windowRef.GetDeltaTime() );
				ImGui::Separator();
			}
			ImGui::End();
		}

		if ( m_showDemoWindow )
		{
			ImGui::ShowDemoWindow( &m_showDemoWindow );
		}
		ImGui::End();
	}


}