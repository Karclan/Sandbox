#include "GameComponentDetails.h"

#include "Imgui/imgui.h"

#include "Utils/format.h"
#include "Utils/customformatters.h"

#include "../PlayerComponent.h"
#include "../GameGridComponent.h"	

namespace Editor
{
	void PlayerComponentDetails::BuildDetailsInternal( Game::PlayerComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Player" ) )
		{
			ImGui::DragInt( "Lives", &inComponent.m_Lives );
			{
				bool flag = inComponent.m_Fired;
				ImGui::Checkbox( "Fired", &flag );
			}
		}
	}

	void GameGridComponentDetails::BuildDetailsInternal( Game::GameGridComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Grid" ) )
		{
			bool dimensionsChanged = ImGui::DragInt( "Width", &inComponent.Width );
			dimensionsChanged |= ImGui::DragInt( "Height", &inComponent.Height );
			
			inComponent.DimensionsChanged = dimensionsChanged;
			ImGui::Checkbox( "DimensionsChanged", &inComponent.DimensionsChanged );

			ImGui::Checkbox("Visualize", &inComponent.Visualize);
			ImGui::DragFloat4( "Debug Colour", &inComponent.DebugColour.x, 0.1f );

			ImGui::Text( fmt::format( "TopLeft: {}", inComponent.TopLeft ).c_str() );
			ImGui::Text( fmt::format( "BottomRight: {}", inComponent.BottomRight ).c_str() );
		}
	}

}
