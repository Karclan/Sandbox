#pragma once

#include "Input/InputAction.h"

namespace Game
{
	struct PlayerInputActions
	{
		static Input::ActionId const MoveLeft;
		static Input::ActionId const MoveRight;
		static Input::ActionId const FireGun;
	};

	struct MenuInputActions
	{
		static Input::ActionId const CursorDown;
		static Input::ActionId const CursorUp;
		static Input::ActionId const SelectOption;
		static Input::ActionId const BackOut;
	};	
}
