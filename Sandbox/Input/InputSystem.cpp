#include "InputSystem.h"

#include "InputAction.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/Logger.h"
#include "../Utils/format.h"
#include "../Utils/customformatters.h"

#include "../magic_enum/magic_enum.hpp"

#include "../Graphics/RenderStats.h"
#include "../Graphics/CameraComponent.h"
#include "../Graphics/CameraStatsComponent.h"

#include "glm/glm.hpp"

#include "imgui.h"
#include "InputStatsComponent.h"


namespace Input
{
	InputSystem::InputSystem( Core::Registry & registry ) noexcept
		: Core::Subsystem( registry )
	{
		m_registry.ctx().emplace<InputStatsComponent>();
	}

	void InputSystem::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
		//We will build the input debug window here
		//Go over every action ID we have in our map
		//display out its status
		//display out every binding mapping and the keycodes
		//Also show out the key code status values

		InputStatsComponent & inputStats = m_registry.ctx().get<InputStatsComponent>();

		if ( ImGui::Begin( "Input System" ) )
		{
			if ( ImGui::CollapsingHeader( "Peepeepoopoo" ) )
			{
				Graphics::RenderStats const renderStats = m_registry.ctx().get<Graphics::RenderStats const>();

				if ( renderStats.m_renderingSubwindow )
				{
					InputStatsComponent const inputStats = m_registry.ctx().get<InputStatsComponent const>();
					m_newMouseState.m_position.x = inputStats.ViewPortCursorPos.x;
					m_newMouseState.m_position.y = inputStats.ViewPortCursorPos.y;

					glm::ivec2 const & minPos = renderStats.ViewportStartingPosition;
					glm::ivec2 const & maxPos = minPos + renderStats.ViewportDimensions;
					glm::dvec2 const & pos = m_newMouseState.m_position;
					m_newMouseState.m_IsInViewport = ( pos.x >= minPos.x && pos.x <= maxPos.x ) && ( pos.y >= minPos.y && pos.y <= maxPos.y );
				}
				else
				{
					glm::ivec2 const & minPos = renderStats.ScreenStartingPosition;
					glm::ivec2 const & maxPos = minPos + renderStats.ScreenDimensions;
					glm::dvec2 const & pos = m_newMouseState.m_position;
					m_newMouseState.m_IsInViewport = ( pos.x >= minPos.x && pos.x <= maxPos.x ) && ( pos.y >= minPos.y && pos.y <= maxPos.y );
				}



				Input::InputStatsComponent const & inputStats = m_registry.ctx().get<Input::InputStatsComponent const>();
				ImGui::Text( fmt::format( "Viewport Cursor Pos: {}, {}", inputStats.ViewPortCursorPos.x, inputStats.ViewPortCursorPos.y ).c_str() );

				auto mouseStateDetails = [] ( const char * StateName, Input::MouseState const & mouseState )
					{
						ImGui::Text( StateName );
						ImGui::Text( fmt::format( "Position: {}", mouseState.m_position ).c_str() );
						ImGui::Text( fmt::format( "World Position: {}", mouseState.m_worldPosition ).c_str() );
						//ImGui::Text( fmt::format( "Scroll: {}", mouseState.m_scroll ).c_str() );
						bool flag = mouseState.m_IsInViewport;
						ImGui::Checkbox( "In Viewport", &flag );
						flag = mouseState.m_IsInWindow;
						ImGui::Checkbox( "In Window", &flag );
					};

				ImGui::Separator();
				mouseStateDetails( "New Mouse", inputStats.m_newMouseState );
				ImGui::Separator();
				mouseStateDetails( "Old Mouse", inputStats.m_oldMouseState );
			}
		}
		ImGui::End();
		
	}

	void InputSystem::FixedUpdate( double deltaTime ) noexcept
	{
		auto lerp = [] ( auto a, auto b, double t )
		{
			//Linear interpolation formula
			//a + (b - a) * t
			return a + ( b - a ) * t;
		};
		m_lerpedMouseState.m_position.x = lerp( m_newMouseState.m_position.x, m_oldMouseState.m_position.x, deltaTime );
		m_lerpedMouseState.m_position.y = lerp( m_newMouseState.m_position.y, m_oldMouseState.m_position.y, deltaTime );
		m_lerpedMouseState.m_scroll.x = lerp( m_newMouseState.m_scroll.x, m_oldMouseState.m_scroll.x, deltaTime );
		m_lerpedMouseState.m_scroll.y = lerp( m_newMouseState.m_scroll.y, m_oldMouseState.m_scroll.y, deltaTime );
		m_lerpedMouseState.m_worldPosition.x = lerp( m_newMouseState.m_worldPosition.x, m_oldMouseState.m_worldPosition.x, deltaTime );
		m_lerpedMouseState.m_worldPosition.y = lerp( m_newMouseState.m_worldPosition.y, m_oldMouseState.m_worldPosition.y, deltaTime );
		m_lerpedMouseState.m_worldPosition.z = lerp( m_newMouseState.m_worldPosition.z, m_oldMouseState.m_worldPosition.z, deltaTime );
	}

	void InputSystem::Update( double deltaTime ) noexcept
	{
		UNUSED( deltaTime );
		//////////////////////////////////////////////////////////////////////////
		//Mouse position
		//////////////////////////////////////////////////////////////////////////
		Graphics::RenderStats const renderStats = m_registry.ctx().get<Graphics::RenderStats const>();

		//if ( renderStats.m_renderingSubwindow )
		//{
		//	InputStatsComponent const inputStats = m_registry.ctx().get<InputStatsComponent const>();
		//	m_newMouseState.m_position.x = inputStats.ViewPortCursorPos.x;
		//	m_newMouseState.m_position.y = inputStats.ViewPortCursorPos.y;

		//	glm::ivec2 const & minPos = renderStats.ViewportStartingPosition;
		//	glm::ivec2 const & maxPos = minPos + renderStats.ViewportDimensions;
		//	glm::dvec2 const & pos = m_newMouseState.m_position;
		//	m_newMouseState.m_IsInViewport = ( pos.x >= minPos.x && pos.x <= maxPos.x ) && ( pos.y >= minPos.y && pos.y <= maxPos.y );
		//}
		//else
		//{
		//	glm::ivec2 const & minPos = renderStats.ScreenStartingPosition;
		//	glm::ivec2 const & maxPos = minPos + renderStats.ScreenDimensions;
		//	glm::dvec2 const & pos = m_newMouseState.m_position;
		//	m_newMouseState.m_IsInViewport = ( pos.x >= minPos.x && pos.x <= maxPos.x ) && ( pos.y >= minPos.y && pos.y <= maxPos.y );
		//}

		m_newMouseState.m_worldPosition = ScreenToWorldSpace( m_newMouseState.m_position );

		m_deltaMouseState.m_position = m_newMouseState.m_position - m_oldMouseState.m_position;
		m_deltaMouseState.m_scroll = m_newMouseState.m_scroll - m_oldMouseState.m_scroll;
		m_deltaMouseState.m_worldPosition = m_newMouseState.m_worldPosition - m_oldMouseState.m_worldPosition;

		m_oldMouseState = m_newMouseState;

		InputStatsComponent & inputStats = m_registry.ctx().get<InputStatsComponent>();
		inputStats.m_newMouseState = m_newMouseState;
		inputStats.m_oldMouseState = m_oldMouseState;
		inputStats.m_lerpedMouseState = m_lerpedMouseState;
		inputStats.m_deltaMouseState = m_deltaMouseState;
		//////////////////////////////////////////////////////////////////////////
		//Input actions
		//////////////////////////////////////////////////////////////////////////
	}

	void InputSystem::PostUpdate( double deltaTime ) noexcept
	{
		//Age mouse inputs
		DeviceId constexpr deviceId = { 1, DeviceType::Mouse };
		KeyCodeActionIdMap const & keyCodeActionIdMap = GetDeviceBinding( deviceId ).GetKeyCodeActionIdBinding();
		for ( auto const & keyCodeIterator : keyCodeActionIdMap )
		{
			ActionId const & actionId = keyCodeIterator.second;
			switch ( m_actionStates[actionId].State )
			{
				case ActionState::Pressed:
					m_actionStates[actionId].State = ActionState::Repeat;
					break;
				case ActionState::Released:
					m_actionStates[actionId].State = ActionState::Unknown;
					break;
			}
		}
	}

	void InputSystem::KeyAction( KeyCode key, int scancode, ActionState action, int mods )
	{
		Utils::Logger::LogMessage( Utils::LogType::Info, fmt::format( "KeyAction {}, Action State {}\n", key, static_cast< int >( action ) ) );

		DeviceId constexpr deviceId = { 0, DeviceType::Keyboard };
		//Key values are 1 or 0 if the key is released or not.
		SetActionInternal( deviceId, key, { action, ( action != ActionState::Released ) ? 1.0f : 0.f } );
	}

	void InputSystem::MouseAction( KeyCode button, ActionState action, int mods )
	{
		//Mouse buttons are 1 or 0 if pressed or released
		DeviceId constexpr deviceId = { 1, DeviceType::Mouse };
		//Key values are 1 or 0 if the key is released or not.
		Utils::Logger::LogMessage( Utils::LogType::Info, fmt::format( "Mouse button {}, Action State {}\n", button, static_cast<int>(action) ) );
		SetActionInternal( deviceId, button, { action, ( action != ActionState::Released ) ? 1.0f : 0.f } );
	}

	void InputSystem::UpdateCursorPosition( double xPosition, double yPosition )
	{
		m_newMouseState.m_position = { xPosition, yPosition };

		Graphics::RenderStats const renderStats = m_registry.ctx().get<Graphics::RenderStats const>();

		m_newMonitorMouseState.m_position = m_newMouseState.m_position + static_cast<glm::dvec2>(renderStats.ViewportStartingPosition);
	}

	void InputSystem::ScrollAction( double xOffset, double yOffset )
	{
		m_newMouseState.m_scroll = { xOffset, yOffset };
	}

	void InputSystem::OnCursorEntered( bool Entered )
	{
		m_newMouseState.m_IsInWindow = Entered;
	}

	bool InputSystem::IsPressesd( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).State == ActionState::Pressed;
	}

	bool InputSystem::IsHeld( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).State == ActionState::Repeat;
	}

	bool InputSystem::IsReleased( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).State == ActionState::Released;
	}

	float InputSystem::GetValue( ActionId const actionId ) const
	{
		assert( m_actionStates.find( actionId ) != m_actionStates.end() );
		return m_actionStates.at( actionId ).Value;
	}

	glm::dvec2 InputSystem::GetMousePosition() const
	{
		return m_newMouseState.m_position;
	}

	glm::dvec3 InputSystem::GetMouseWorldPosition() const
	{
		return m_newMouseState.m_worldPosition;
	}

	glm::dvec3 InputSystem::GetOldMouseWorldPosition() const
	{
		return m_oldMouseState.m_worldPosition;
	}

	glm::dvec3 InputSystem::GetDeltaMouseWorldPosition() const
	{
		return m_deltaMouseState.m_worldPosition;
	}

	glm::dvec3 InputSystem::GetLerpedMouseWorldPosition() const
	{
		return m_lerpedMouseState.m_worldPosition;
	}

	void InputSystem::AddBinding( std::shared_ptr<DeviceBindings> & Mapping )
	{
		if ( Mapping )
		{
			Mapping->SetupBindings();

			DeviceId const MappingId = Mapping->GetDeviceId();
			//Go through the mapping and add the ActionId into our action states.
			//Default initialize entries so we can query them.
			KeyCodeActionIdMap const & Bindings = Mapping->GetKeyCodeActionIdBinding();
			for ( auto const & binding : Bindings )
			{
				ActionId const & action = binding.second;
				m_actionStates[ action ] = {};
			}

			m_deviceBindings.emplace( MappingId, Mapping );

			DeviceBindingsContainer::iterator mergedMappingIterator = m_mergedDeviceBindings.find( MappingId );
			if ( mergedMappingIterator != m_mergedDeviceBindings.end() )
			{
				std::shared_ptr<DeviceBindings> MergedBinding = mergedMappingIterator->second;
				MergedBinding->Merge( *Mapping.get() );
			}
			else
			{
				m_mergedDeviceBindings.emplace( MappingId, Mapping );
			}
		}
	}

	glm::dvec3 InputSystem::ScreenToWorldSpace( glm::dvec2 const & position ) const
	{
		Graphics::RenderStats const & renderStats = m_registry.GetConstContextComponent<Graphics::RenderStats const>();
		Graphics::CameraStats const & cameraStats = m_registry.ctx().get<Graphics::CameraStats const>();

		if ( cameraStats.m_mainCameraEntity != Core::NullEntity )
		{
			Graphics::CameraComponent const & cameraComponent = m_registry.GetConstComponent<Graphics::CameraComponent const>( cameraStats.m_mainCameraEntity );
			double const width = renderStats.ViewportDimensions.x;
			double const height = renderStats.ViewportDimensions.y;

			double const x = 2.0 * position.x / width - 1;
			double const y = 2.0 * position.y / height - 1;

			glm::vec4 const screenPosition( x, -y, -1.0, 1.0 );

			glm::mat4 const projViewMatrix = cameraComponent.m_projectionMatrix * cameraComponent.m_viewMatrix;
			glm::mat4 const inverseProjViewMatrix = glm::inverse( projViewMatrix );

			glm::vec4 const worldPosition = inverseProjViewMatrix * screenPosition;

			return glm::dvec3( worldPosition );
		}
		return { 0.f, 0.f, 0.f };
	}

	DeviceBindings const & InputSystem::GetDeviceBinding( DeviceId const & deviceId ) const
	{
		DeviceBindingsContainer::const_iterator const it = m_mergedDeviceBindings.find( deviceId );
		if ( it != m_mergedDeviceBindings.end() )
		{
			return *it->second.get();
		}
		else
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, fmt::format( "No bindings found for Device id {} of type {}\n", deviceId.m_id, std::string( magic_enum::enum_name( deviceId.m_type ) ) ) );
			return m_dummyDeviceBindings;
		}
	}

	bool InputSystem::SetActionInternal( DeviceId const & deviceId, KeyCode key, InputState const & state )
	{
		KeyCodeActionIdMap const & keyCodeActionIdMap = GetDeviceBinding( deviceId ).GetKeyCodeActionIdBinding();
		KeyCodeActionIdMap::const_iterator const keyCodeIterator = keyCodeActionIdMap.find( key );
		if ( keyCodeIterator != keyCodeActionIdMap.end() )
		{
			ActionId const & actionId = keyCodeIterator->second;
			m_actionStates[ actionId ] = state;
			return true;
		}
		return false;
	}

	InputState InputSystem::GetActionStateInternal( DeviceId const & deviceId, KeyCode key )
	{
		KeyCodeActionIdMap const & keyCodeActionIdMap = GetDeviceBinding( deviceId ).GetKeyCodeActionIdBinding();
		KeyCodeActionIdMap::const_iterator const keyCodeIterator = keyCodeActionIdMap.find( key );
		if ( keyCodeIterator != keyCodeActionIdMap.end() )
		{
			ActionId const & actionId = keyCodeIterator->second;
			return m_actionStates[actionId];
		}
		return {};
	}

}
