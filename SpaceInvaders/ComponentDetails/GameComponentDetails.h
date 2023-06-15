#pragma once

#include "Editor/ComponentDetails/ComponentDetailsBase.h"
#include "Editor/ComponentDetails/GenericTagsDetails.h"

#include "../GameTags.h"

namespace Game
{
	struct PlayerComponent;
}

namespace Editor
{
	class PlayerComponentDetails : public Editor::TComponentDetails<Game::PlayerComponent>
	{
	protected:
		void BuildDetailsInternal( Game::PlayerComponent & inComponent ) const override;
	};

	class PlayerTagDetails : public Editor::TTagDetails<Game::PlayerTag, "Player">
	{
	};
}
