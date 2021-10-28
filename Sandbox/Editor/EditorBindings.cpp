#include "EditorBindings.h"

#include "InputActions.h"

#include "GLFW/glfw3.h"

namespace Editor
{
	EditorMouseBindings::EditorMouseBindings() noexcept
		:Input::DeviceBindings( Input::DeviceId( 0, { Input::DeviceType::Mouse } ) )
	{
	}

	void EditorMouseBindings::SetupBindings()
	{
		using namespace Input::Literals;
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_MOUSE_BUTTON_1 ), Actions::CameraPanToggle );
	}

	EditorKeyboardBindings::EditorKeyboardBindings() noexcept
		:Input::DeviceBindings( Input::DeviceId( 0, { Input::DeviceType::Keyboard } ) )
	{
	}

	void EditorKeyboardBindings::SetupBindings()
	{
		using namespace Input::Literals;
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_W ), Actions::CameraPanUp );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_S ), Actions::CameraPanDown );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_A ), Actions::CameraPanLeft );
		m_keycodeActionIdMap.emplace( static_cast< Input::KeyCode >( GLFW_KEY_D ), Actions::CameraPanRight );
	}

}