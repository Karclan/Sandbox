#pragma once

#include "Core/Subsystem.h"

namespace Game
{
	class EnemyControllerSubsystem : public Core::Subsystem
	{

	public:
		EnemyControllerSubsystem( Core::Registry & registry );
		~EnemyControllerSubsystem() = default;

		void FixedUpdate( double deltaTime ) override;
		void Update( double deltaTime ) override; 

	private:
		void CreateComponents();
	};
}
