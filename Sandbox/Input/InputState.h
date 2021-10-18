#pragma once

#include "glm/ext/vector_double2.hpp"

#include <map>

namespace Input
{
	typedef typename int KeyCode;

	enum class ActionState : int
	{
		Unknown = -1,
		Pressed = 0,
		Released,
		Repeat
	};

	struct InputState
	{
		ActionState m_state{ ActionState::Unknown };
		float m_value{ 0.f };
	};

	using KeyCodeActionIdMap = std::map<KeyCode, ActionId>;
	using ActionIdInputState = std::map<ActionId, InputState>;

	struct MouseState
	{
		glm::dvec2 m_position{ 0.f };
		glm::dvec2 m_scroll{ 0.f };
	};
}