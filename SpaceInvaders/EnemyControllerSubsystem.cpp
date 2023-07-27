#include "EnemyControllerSubsystem.h"
#include "Utils/UnusedMacro.h"
#include "Utils/format.h"

#include "Graphics/GraphicsDraw.h"

#include "SpriteComponent.h"
#include "glm/glm.hpp"
#include "Graphics/TransformComponent.h"

namespace Game
{

	EnemyControllerSubsystem::EnemyControllerSubsystem( Core::Registry & registry )
		: Core::Subsystem( registry )
	{
		CreateComponents();
	}

	void EnemyControllerSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );

	}

	void EnemyControllerSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );

	}

	void EnemyControllerSubsystem::CreateComponents()
	{
		//Create all the enemies
		//11 * 5

		int score = 500;
		glm::vec3 position { -55.f, 70.f, 0.f };
		//For every column
		for ( int c = 0; c < 5; ++c )
		{
			//For every row
			for ( int r = 0; r < 11; ++r )
			{
				{
					Core::Entity newEnemy = Graphics::GraphicsDraw::CreateSprite( m_registry, fmt::format( "Enemy: [{}, {}]", c, r ).c_str(), "" );
					Graphics::TransformComponent transform;
					transform.Position = position;
					m_registry.AddComponent<Graphics::TransformComponent>( newEnemy, transform );
					
				}
				position.x += 10.f;
			}

			position.x = -55.f; //reset the position
			//Lower the height positon each row
			position.y -= 10.f;
			//Lower the score each row
			score -= 100;
		}
	}

}
