#include "GameComponentDetails.h"

#include "Imgui/imgui.h"

#include "../PlayerComponent.h"

namespace Editor
{
	void PlayerComponentDetails::BuildDetailsInternal( Game::PlayerComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Player" ) )
		{
			ImGui::DragInt( "Lives", &inComponent.m_Lives );
		}
	}
}
