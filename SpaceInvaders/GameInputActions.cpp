#include "GameInputActions.h"

using namespace Input::Literals;

namespace Game
{
	Input::ActionId const PlayerInputActions::MoveLeft( "Player_Move_Left"_ActionId );
	Input::ActionId const PlayerInputActions::MoveRight( "Player_Move_Right"_ActionId );
	Input::ActionId const PlayerInputActions::FireGun( "Player_Fire_Gun"_ActionId );
	Input::ActionId const PlayerInputActions::AddSand( "AddSand"_ActionId );

	Input::ActionId const MenuInputActions::CursorDown( "Menu_Cursor_Down"_ActionId );
	Input::ActionId const MenuInputActions::CursorUp( "Menu_Cursor_Up"_ActionId );
	Input::ActionId const MenuInputActions::SelectOption( "Menu_Select_Option"_ActionId );
	Input::ActionId const MenuInputActions::BackOut( "Menu_Back_Out"_ActionId );
}
