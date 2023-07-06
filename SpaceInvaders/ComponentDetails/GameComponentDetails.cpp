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

				ImGui::Separator();
			}
			ImGui::DragFloat( "Movement Speed", &inComponent.m_MovementSpeed, 0.1f );
			
			ImGui::DragFloat3( "CirclePos", &inComponent.CirclePos.x, 0.1f );
			ImGui::DragFloat( "CircleRadius", &inComponent.CircleRadius, 0.1f );
			ImGui::DragFloat4( "CircleColour", &inComponent.CircleColour.x, 0.1f );
			ImGui::DragInt( "CirclePointCount", &inComponent.CirclePointCount, 1.f, 2, 360 );
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
