#pragma once

namespace Game
{
	struct PlayerComponent
	{
		int m_Lives { 0 };
		float m_MovementSpeed { 10 };
		bool m_Fired { false };

		glm::vec3 CirclePos { 0.f };
		float CircleRadius { 1.f };
		glm::vec4 CircleColour { 1.f, 1.f, 1.f, 1.f };
		int CirclePointCount { 3 };
	};
}