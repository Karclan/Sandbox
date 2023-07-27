#pragma once

namespace Game
{
	enum class EnemyType : int
	{
		Invader,
		Mothership,
	};

	struct EnemyComponent
	{
		int ScoreReward {10};
	};
}
