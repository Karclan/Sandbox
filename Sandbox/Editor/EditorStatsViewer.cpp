#include "EditorStatsViewer.h"

#include "../Graphics/CameraStatsComponent.h"
#include "../Graphics/RenderStats.h"

#include "../Utils/format.h"
#include "../Editor/EditorContextComponent.h"

#include "../Input/InputStatsComponent.h"

#include "../Utils/customformatters.h"

#include "imgui.h"

namespace Editor
{
	EditorStatsViewer::EditorStatsViewer( Core::Registry & registry )
		: Core::Subsystem( registry )
	{}

	void EditorStatsViewer::BuildWindow( double deltaTime )
	{
		if ( ImGui::Begin( "Editor Stats" ) )
		{
			if ( ImGui::CollapsingHeader( "Camera Stats" ) )
			{
				Graphics::CameraStats const & cameraStats = m_registry.ctx().get<Graphics::CameraStats const>();
				ImGui::Text( fmt::format( "Main Camera Entity: {}", cameraStats.m_mainCameraEntity ).c_str() );
			}

			if ( ImGui::CollapsingHeader( "Render Stats" ) )
			{
				Graphics::RenderStats const & renderStats = m_registry.ctx().get<Graphics::RenderStats const>();

				ImGui::Text( fmt::format( "Framebuffer Dimensions: {}", renderStats.FrameBufferDimensions ).c_str() );
				ImGui::Text( fmt::format( "Framebuffer Starting Position: {}", renderStats.FrameBufferDimensions ).c_str() );

				ImGui::Text( fmt::format( "Screen Dimensions: {}", renderStats.ScreenDimensions ).c_str() );
				ImGui::Text( fmt::format( "Screen Starting Position: {}", renderStats.ScreenStartingPosition ).c_str() );

				bool flag = renderStats.m_renderingSubwindow;
				ImGui::Checkbox( "Rendering into window", &flag );

				ImGui::Separator();
				ImGui::Text( flag ? "Using subwindow sizes" : "Using screen sizes" );
				ImGui::Text( fmt::format( "Viewport Dimensions: {}", renderStats.ViewportDimensions ).c_str() );
				ImGui::Text( fmt::format( "Viewport Starting Position: {}", renderStats.ViewportStartingPosition ).c_str() );

				ImGui::Text( fmt::format( "Viewport Ending Position: {}", renderStats.ViewportStartingPosition + renderStats.ViewportDimensions ).c_str() );
			}

			if ( ImGui::CollapsingHeader( "Editor Stats" ) )
			{
				EditorContextComponent const & editorContext = m_registry.ctx().get<EditorContextComponent const>();
				ImGui::Text( fmt::format( "Inspected Entity: {}", editorContext.m_InspectedEntity ).c_str() );
			}

			if ( ImGui::CollapsingHeader( "Input Stats" ) )
			{
				Input::InputStatsComponent const & inputStats = m_registry.ctx().get<Input::InputStatsComponent const>();
				ImGui::Text( fmt::format( "Viewport Cursor Pos: {}, {}", inputStats.ViewPortCursorPos.x, inputStats.ViewPortCursorPos.y ).c_str() );

				auto mouseStateDetails = [] ( const char * StateName, Input::MouseState const & mouseState )
					{
						ImGui::Text( StateName );
						ImGui::Text( fmt::format( "Position: {}", mouseState.m_position ).c_str() );
						ImGui::Text( fmt::format( "World Position: {}", mouseState.m_worldPosition ).c_str() );
						//ImGui::Text( fmt::format( "Scroll: {}", mouseState.m_scroll ).c_str() );
						bool flag = mouseState.m_IsInViewport;
						ImGui::Checkbox( "In Viewport", &flag );
						flag = mouseState.m_IsInWindow;
						ImGui::Checkbox( "In Window", &flag );
					};

				ImGui::Separator();
				mouseStateDetails( "New Mouse", inputStats.m_newMouseState );
				ImGui::Separator();
				mouseStateDetails( "Old Mouse", inputStats.m_oldMouseState );
				/*		ImGui::Separator();
						mouseStateDetails( "Delta Mouse", inputStats.m_deltaMouseState );
						ImGui::Separator();
						mouseStateDetails( "Lerped Mouse", inputStats.m_lerpedMouseState );*/
			}

		}
		ImGui::End();
	}
}
