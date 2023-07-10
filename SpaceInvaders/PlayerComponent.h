#pragma once

namespace Game
{
	struct PlayerComponent
	{
		int m_Lives { 0 };
		float m_MovementSpeed { 10 };
		bool m_Fired { false };
	};
}