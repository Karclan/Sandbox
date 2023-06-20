#include "EditorBindings.h"

#include "InputActions.h"

#include "GLFW/glfw3.h"

namespace Editor
{
	void EditorMouseBindings::SetupBindings()
	{
		/*m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_MOUSE_BUTTON_1 ), Actions::CameraPanToggle );*/
	}

	void EditorKeyboardBindings::SetupBindings()
	{
		/*m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_UP ), Actions::CameraPanUp );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_DOWN ), Actions::CameraPanDown );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_LEFT ), Actions::CameraPanLeft );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_RIGHT ), Actions::CameraPanRight );*/
	}
}