#pragma once

#include "../Core/Subsystem.h"

#include "InputAction.h"

#include "glm/ext/vector_double2.hpp"

#include <vector>
#include <map>

namespace Input
{
	class InputSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		InputSystem(Core::Registry& registry);
		~InputSystem() = default;

		bool TestAction(ActionId const& action) const;

		void FixedUpdate(double deltaTime) noexcept override;
		void Update(double deltaTime) noexcept override;
		void BuildWindow(double deltaTime) override;

		enum class ActionState : int
		{
			Unknown = -1,
			Pressed = 0,
			Released,
			Repeat
		};

		//Functions which set various keycode states
		void KeyAction(int key, int scancode, ActionState action, int mods);
		void MouseAction(int button, ActionState action, int mods);
		void UpdateCursorPosition(double xPosition, double yPosition);
		void ScrollAction(double xOffset, double yOffset);

		//Input state querying functions
		bool IsPressesd(ActionId const actionId) const;
		bool IsHeld(ActionId const actionId) const;
		bool IsReleased(ActionId const actionId) const;
		float GetValue(ActionId const actionId) const;

	private:

		//Temporary function for setting up bindings between keycodes and action ids
		//In the future I will be having an external file which specifies the bindings
		//which can then be readin and setup
		void SetupKeycodeBinding();

		using KeyCode = int;
		struct InputState
		{
			ActionState m_state{ ActionState::Unknown };
			float m_value{ 0.f };
		};

		enum class DeviceType : int
		{
			Invalid = -1,
			Mouse = 0,
			Keyboard,
			Controller
		};

		struct DeviceTypeKeyCodePair
		{
			bool operator<(DeviceTypeKeyCodePair const& Other) const
			{
				//If device ids match compare on codes.
				if (device == Other.device)
				{
					return code < Other.code;
				}
				else
				{
					return device < Other.device;
				}
			}
			DeviceType device{ DeviceType::Invalid };
			KeyCode code{ 0 };
		};

		struct MouseState
		{
			glm::dvec2 m_position{ 0.f };
			glm::dvec2 m_scroll{ 0.f };
		};

		MouseState m_oldMouseState{};
		MouseState m_deltaMouseState{};
		MouseState m_lerpedMouseState{};
		MouseState m_newMouseState{};

		//Map of action ids to keycodes, used to find the input state
		std::map<ActionId, KeyCode> m_actionKeyCodeMapping{};
		//Map of keycodes to input states
		std::map<DeviceTypeKeyCodePair, InputState> m_keyCodeMapping{};
	};
}
