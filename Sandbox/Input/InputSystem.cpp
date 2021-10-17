#include "InputSystem.h"

#include "InputAction.h"
#include "../Utils/UnusedMacro.h"

namespace Input
{
	InputSystem::InputSystem(Core::Registry& registry)
		: Core::Subsystem(registry)
	{

	}

	bool InputSystem::TestAction(ActionId const& action) const
	{
		using namespace Literals;
		switch (action)
		{
		case "Test"_ActionId:
		{

		}
		}

		return false;
	}

	void InputSystem::BuildWindow(double deltaTime)
	{
		UNUSED(deltaTime);
	}

	void InputSystem::FixedUpdate(double deltaTime) noexcept
	{
		m_lerpedMouseState.m_position.x = std::lerp(m_newMouseState.m_position.x, m_oldMouseState.m_position.x, deltaTime);
		m_lerpedMouseState.m_position.y = std::lerp(m_newMouseState.m_position.y, m_oldMouseState.m_position.y, deltaTime);
		m_lerpedMouseState.m_scroll.x = std::lerp(m_newMouseState.m_scroll.x, m_oldMouseState.m_scroll.x, deltaTime);
		m_lerpedMouseState.m_scroll.y = std::lerp(m_newMouseState.m_scroll.y, m_oldMouseState.m_scroll.y, deltaTime);
	}

	void InputSystem::Update(double deltaTime) noexcept
	{
		UNUSED(deltaTime);
		m_deltaMouseState.m_position = m_newMouseState.m_position - m_oldMouseState.m_position;
		m_deltaMouseState.m_scroll = m_newMouseState.m_scroll - m_oldMouseState.m_scroll;

		m_oldMouseState = m_newMouseState;
	}

	void InputSystem::KeyAction(int key, int scancode, ActionState action, int mods)
	{
		//Key values are 1 or 0 if the key is released or not.
		DeviceTypeKeyCodePair DeviceKey{ DeviceType::Keyboard, key };
		m_keyCodeMapping[DeviceKey] = { action, (action != ActionState::Released) ? 1.0f : 0.f };
	}

	void InputSystem::MouseAction(int button, ActionState action, int mods)
	{
		//Mouse buttons are 1 or 0 if pressed or released
		DeviceTypeKeyCodePair DeviceKey{ DeviceType::Mouse, button };
		m_keyCodeMapping[DeviceKey] = { action, (action != ActionState::Released) ? 1.0f : 0.f };
	}

	void InputSystem::UpdateCursorPosition(double xPosition, double yPosition)
	{
		m_newMouseState.m_position= { xPosition, yPosition };
	}

	void InputSystem::ScrollAction(double xOffset, double yOffset)
	{
		m_newMouseState.m_scroll = { xOffset, yOffset };
	}

	bool InputSystem::IsPressesd(ActionId const actionId) const
	{
		return false;
	}

	bool InputSystem::IsHeld(ActionId const actionId) const
	{
		return false;
	}

	bool InputSystem::IsReleased(ActionId const actionId) const
	{
		return false;
	}

	float InputSystem::GetValue(ActionId const actionId) const
	{
		return 0.0f;
	}


}
