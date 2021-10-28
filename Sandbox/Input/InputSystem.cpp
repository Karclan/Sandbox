#include "InputSystem.h"

#include "InputAction.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/Logger.h"
#include "../Utils/format.h"

#include "../magic_enum/magic_enum.hpp"

namespace Input
{
	InputSystem::InputSystem( Core::Registry & registry ) noexcept
		: Core::Subsystem( registry )
	{

	}
	
	void InputSystem::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
		//We will build the input debug window here
		//Go over every action ID we have in our map
		//display out its status
		//display out every binding mapping and the keycodes
		//Also show out the key code status values
	}

	void InputSystem::FixedUpdate( double deltaTime ) noexcept
	{
		m_lerpedMouseState.m_position.x = std::lerp( m_newMouseState.m_position.x, m_oldMouseState.m_position.x, deltaTime );
		m_lerpedMouseState.m_position.y = std::lerp( m_newMouseState.m_position.y, m_oldMouseState.m_position.y, deltaTime );
		m_lerpedMouseState.m_scroll.x = std::lerp( m_newMouseState.m_scroll.x, m_oldMouseState.m_scroll.x, deltaTime );
		m_lerpedMouseState.m_scroll.y = std::lerp( m_newMouseState.m_scroll.y, m_oldMouseState.m_scroll.y, deltaTime );
	}

	void InputSystem::Update( double deltaTime ) noexcept
	{
		UNUSED( deltaTime );
		m_deltaMouseState.m_position = m_newMouseState.m_position - m_oldMouseState.m_position;
		m_deltaMouseState.m_scroll = m_newMouseState.m_scroll - m_oldMouseState.m_scroll;

		m_oldMouseState = m_newMouseState;
	}

	void InputSystem::KeyAction( KeyCode key, int scancode, ActionState action, int mods )
	{
		DeviceId constexpr keyboardId = { 0, DeviceType::Keyboard };
		//Key values are 1 or 0 if the key is released or not.
		SetActionInternal( keyboardId, key, { action, ( action != ActionState::Released ) ? 1.0f : 0.f } );
	
		/*DeviceTypeKeyCodePair DeviceKey{ DeviceType::Keyboard, key };
		m_keyCodeMapping[DeviceKey] = { action, ( action != ActionState::Released ) ? 1.0f : 0.f };*/
	}

	void InputSystem::MouseAction( KeyCode button, ActionState action, int mods )
	{
		//Mouse buttons are 1 or 0 if pressed or released
		DeviceId constexpr keyboardId = { 0, DeviceType::Mouse };
		//Key values are 1 or 0 if the key is released or not.
		SetActionInternal( keyboardId, button, { action, ( action != ActionState::Released ) ? 1.0f : 0.f } );

		/*DeviceTypeKeyCodePair DeviceKey { DeviceType::Mouse, button };
		m_keyCodeMapping[ DeviceKey ] = { action, ( action != ActionState::Released ) ? 1.0f : 0.f };*/
	}

	void InputSystem::UpdateCursorPosition( double xPosition, double yPosition )
	{
		m_newMouseState.m_position = { xPosition, yPosition };
	}

	void InputSystem::ScrollAction( double xOffset, double yOffset )
	{
		m_newMouseState.m_scroll = { xOffset, yOffset };
	}

	bool InputSystem::IsPressesd( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).m_state == ActionState::Pressed;
	}

	bool InputSystem::IsHeld( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).m_state == ActionState::Repeat;
	}

	bool InputSystem::IsReleased( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).m_state == ActionState::Released;
	}

	float InputSystem::GetValue( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).m_value;
	}

	void InputSystem::AddBinding( DeviceId inId, DeviceBindings && Mapping )
	{
		m_deviceBindings.emplace( inId, std::move( Mapping ) );
	}

	DeviceBindings const & InputSystem::GetDeviceBinding( DeviceId const & deviceId ) const
	{
		DeviceBindingsContainer::const_iterator const it = m_deviceBindings.find( deviceId );
		if ( it != m_deviceBindings.end() )
		{
			return it->second;
		}
		else
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, fmt::format( "No bindings found for Device id {} of type {}\n", deviceId.m_id, std::string(magic_enum::enum_name(deviceId.m_type)) ) );
			return m_dummyDeviceBindings;
		}
	}

	bool InputSystem::SetActionInternal( DeviceId const & deviceId, KeyCode key, InputState const & state )
	{
		DeviceBindings const & binding = GetDeviceBinding( deviceId );
		KeyCodeActionIdMap const & keycodeMap = binding.GetKeyCodeActionIdBinding();
		KeyCodeActionIdMap::const_iterator const keyCodeIterator = keycodeMap.find( key );
		if ( keyCodeIterator != keycodeMap.end() )
		{
			ActionId const & actionId = keyCodeIterator->second;
			m_actionStates[ actionId ] = state;
			return true;
		}
		return false;
	}
}
