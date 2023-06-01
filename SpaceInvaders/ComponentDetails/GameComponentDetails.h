#pragma once

#include "Editor/ComponentDetails/ComponentDetailsBase.h"

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
}
