#pragma once

#include "../Core/Subsystem.h"

#include "InputAction.h"
#include "Device.h"
#include "InputState.h"
#include "DeviceBindings.h"

namespace Input
{
	class InputSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		InputSystem( Core::Registry & registry ) noexcept;
		~InputSystem() = default;

		void FixedUpdate( double deltaTime ) noexcept override;
		void Update( double deltaTime ) noexcept override;
		void BuildWindow( double deltaTime ) override;

		//Functions which set various keycode states
		void KeyAction( KeyCode key, int scancode, ActionState action, int mods );
		void MouseAction( KeyCode button, ActionState action, int mods );
		void UpdateCursorPosition( double xPosition, double yPosition );
		void ScrollAction( double xOffset, double yOffset );

		//Input state querying functions
		bool IsPressesd( ActionId const actionId ) const;
		bool IsHeld( ActionId const actionId ) const;
		bool IsReleased( ActionId const actionId ) const;
		float GetValue( ActionId const actionId ) const;
		//Ugly but I want to change this to something like axis value or w.e
		glm::dvec2 GetMousePosition() const;


		void AddBinding( DeviceId inId, DeviceBindings && Mapping );

	private:
		/*  theory
			action ids link to state values
			devices link key codes to action ids
			devices fill in those action id states based on their latest input
			So that way we can have multiple devices write values to the same action ids

			How do we determine which is the latest device to write to that?
			Unless all our inputs are on callbacks
			but Im unsure how tha tworks in terms of something like controller axis inputs

			It does mean we might have duplicate data but in terms of input that's okay
			as we want action ids to have their own values regardless if they are mapped to the same button
			I think that is better than saving memory on duplicate bindings for matching action ids

			Though perhaps we could somehow do an optimization at a later date
		*/

		//Device has a list of keycode states
		//And a device binding has a list of action ids that link up to a keycode
		//So we can build input on a keycode facing direction
		//and query it on an action id facing direction
		//Build
		//ActiveDevice->KeyCode->SetState
		//ActionId->InputSystem->ActiveDevice->KeyCode->State
		//But what about input from multiple areas?
		//Such as keyboard and mouse??

		//Perhaps a virtual device which is an amalgamation of the different active devices?
		//but then we have to figure out which ones of similar types take priority over others.

		DeviceBindings const & GetDeviceBinding( DeviceId const & keyboardId ) const;

		bool SetActionInternal(DeviceId const & deviceId, KeyCode key, InputState const & state);

		//struct DeviceTypeKeyCodePair
		//{
		//	bool operator<( DeviceTypeKeyCodePair const & Other ) const noexcept
		//	{
		//		//If device ids match compare on codes.
		//		if ( device == Other.device )
		//		{
		//			return code < Other.code;
		//		}
		//		else
		//		{
		//			return device < Other.device;
		//		}
		//	}
		//	DeviceType device { DeviceType::Invalid };
		//	KeyCode code { 0 };
		//};

		MouseState m_oldMouseState {};
		MouseState m_deltaMouseState {};
		MouseState m_lerpedMouseState {};
		MouseState m_newMouseState {};

		using DeviceBindingsContainer = std::map<DeviceId, DeviceBindings>;
		DeviceBindingsContainer m_deviceBindings;

		//Dummy device bindings for when we fail to find bindings within our setup container
		DeviceBindings m_dummyDeviceBindings { { -1, DeviceType::Invalid } };

		ActionIdInputState m_actionStates;
	};
}
