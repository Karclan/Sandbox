#pragma once

namespace Game
{
	struct PlayerComponent
	{
		int m_Lives { 0 };	
		bool m_Fired { false };
	};
}