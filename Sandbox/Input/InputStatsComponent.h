#pragma once

#include "InputState.h"

#include "../Imgui/imgui.h"

namespace Input
{
	struct InputStatsComponent
	{ 
		ImVec2 ViewPortCursorPos { 0.f,0.f };

		MouseState m_oldMouseState {};
		MouseState m_deltaMouseState {};
		MouseState m_lerpedMouseState {};
		MouseState m_newMouseState {};
	};
}
