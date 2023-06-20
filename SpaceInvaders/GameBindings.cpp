#include "GameBindings.h"

#include "GameInputActions.h"

#include "GLFW/glfw3.h"

namespace Game
{
	void GameMouseBindings::SetupBindings()
	{
	}

	void GameKeyboardBindings::SetupBindings()
	{
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_A ), PlayerInputActions::MoveLeft );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_D ), PlayerInputActions::MoveRight );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_LEFT ), PlayerInputActions::MoveLeft );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_RIGHT ), PlayerInputActions::MoveRight );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_SPACE ), PlayerInputActions::FireGun );

		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_S ), MenuInputActions::CursorDown );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_W ), MenuInputActions::CursorUp );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_ENTER ), MenuInputActions::SelectOption );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_ESCAPE ), MenuInputActions::BackOut );
	}

}
