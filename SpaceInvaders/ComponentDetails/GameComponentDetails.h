#pragma once

#include "Editor/ComponentDetails/ComponentDetailsBase.h"
#include "Editor/ComponentDetails/GenericTagsDetails.h"

#include "../GameTags.h"

namespace Game
{
	struct PlayerComponent;
	struct GameGridComponent;
}

namespace Editor
{
	//////////////////////////////////////////////////////////////////////////
	//Components
	//////////////////////////////////////////////////////////////////////////

	class PlayerComponentDetails : public Editor::TComponentDetails<Game::PlayerComponent>
	{
	protected:
		void BuildDetailsInternal( Game::PlayerComponent & inComponent ) const override;
	};

	class GameGridComponentDetails : public Editor::TComponentDetails<Game::GameGridComponent>
	{
	protected:
		void BuildDetailsInternal( Game::GameGridComponent & inComponent ) const override;
	};

	//////////////////////////////////////////////////////////////////////////
	//Tags
	//////////////////////////////////////////////////////////////////////////
	class PlayerTagDetails : public Editor::TTagDetails<Game::PlayerTag, "Player">
	{
	};

}
