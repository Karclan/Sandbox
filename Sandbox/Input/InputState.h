#pragma once

#include "InputAction.h"

#include "glm/ext/vector_double2.hpp"
#include "glm/ext/vector_double3.hpp"

#include <map>
#include <set>

namespace Input
{
	using KeyCode = int;

	enum class ActionState : int
	{
		Released = 0,
		Pressed,
		Repeat,
		Unknown = -1,
	};

	struct InputState
	{
		ActionState State { ActionState::Unknown };
		float Value { 0.f };
	};

	using KeyCodeActionIdMap = std::map<KeyCode, ActionId>;
	using ActionIdInputState = std::map<ActionId, InputState>;

	struct MouseState
	{
		glm::dvec2 m_position { 0.f };
		glm::dvec2 m_scroll { 0.f };

		glm::dvec3 m_worldPosition { 0.f }; //World position of mouse based on current active camera
		bool m_IsInViewport = false;
		bool m_IsInWindow = false;
	};
}