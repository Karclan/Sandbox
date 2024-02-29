#include "EditorStatsViewer.h"

#include "../Graphics/CameraStatsComponent.h"
#include "../Graphics/RenderStats.h"

#include "../Utils/format.h"
#include "../Editor/EditorContextComponent.h"

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
				ImGui::Text( fmt::format( "Screen Starting Position: {}", renderStats.WindowPosition ).c_str() );

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

		}
		ImGui::End();
	}
}
