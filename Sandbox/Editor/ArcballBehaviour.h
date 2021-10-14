#pragma once

#include <glm/glm.hpp>

namespace Editor
{
	struct ArcballBehaviour
	{
		float m_xAngle { 0.f };
		float m_yAngle { 0.f };
		float m_cosAngle { 0.f };

		glm::vec2 m_lastMousePosition { 0.f };
	};
}

