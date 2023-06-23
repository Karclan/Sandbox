#include "CollisionDetectionSystem.h"
#include "CollisionComponent.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"
#include "../Utils/Logger.h"

#include "../Graphics/TransformComponent.h"
#include "../Graphics/Window.h"
#include "../Graphics/GraphicsDraw.h"

namespace Physics
{
	CollisionDetectionSystem::CollisionDetectionSystem( Core::Registry & registry, Graphics::Window & window )
		: Core::Subsystem( registry )
		, m_window( window )
	{
		m_registry.on_construct<SquareExtents>().connect<&CollisionDetectionSystem::OnSquareExtentsConstruct>( this );
	}

	CollisionDetectionSystem::~CollisionDetectionSystem()
	{
		m_registry.on_construct<SquareExtents>().disconnect<&CollisionDetectionSystem::OnSquareExtentsConstruct>( this );
	}

	void CollisionDetectionSystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );

		//Fixed update is when we do the checks?
		//This way we should be able to run multiple checks between render frames as we can have
		//multiple checks between a render as fixed update plays catchup under heavy loads

		m_registry.ForView<CollisionComponent>(
			[] ( CollisionComponent & collision )
			{
				collision.isColliding = false;
			} );

		//Grab all collidables
		auto const CollidableEntitiesView = m_registry.View<Graphics::TransformComponent const, CollisionComponent const>();
		//For every collidable check it against every other collidable.
		//We figure out the collision types after
		//Eventually we might want to have something like an oct-tree to split up the checks so we only check collidables
		//which are likely to collide
		for ( Core::Entity const CollidableEntity : CollidableEntitiesView )
		{
			for ( Core::Entity const OtherCollidableEntity : CollidableEntitiesView )
			{
				if ( CollidableEntity != OtherCollidableEntity )
				{
					CheckCollision( CollidableEntity, OtherCollidableEntity );
				}
			}
		}
	}

	void CollisionDetectionSystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );
		m_registry.ForView<CollisionComponent const, SquareExtents, Graphics::TransformComponent const>( [] ( CollisionComponent const & collision, SquareExtents & extents, Graphics::TransformComponent const & transform )
			{
				glm::vec3 const halfExtents( extents.width / 2.f, extents.height / 2.f, 0 );
				extents.topLeft = transform.Position + glm::vec3( extents.offset, 0.f ) - halfExtents;
				extents.bottomRight = transform.Position + glm::vec3( extents.offset, 0.f ) + halfExtents;
				//extents.topLeft = { transform.Position.x - halfExtents.x, transform.Position.y - halfExtents.y };
				//extents.bottomRight = { transform.Position.x + halfExtents.x, transform.Position.y + halfExtents.y };
			} );

		m_registry.ForView<CollisionComponent const, SquareExtents const, SquareExtentsDebugRenderComponent, Graphics::TransformComponent const>( [this] ( CollisionComponent const & collision, SquareExtents const & extents, SquareExtentsDebugRenderComponent & debugRender, Graphics::TransformComponent const & transform )
			{
				if ( collision.drawDebugLines )
				{
					float const zPosMinus1 = transform.Position.z - 1.f; //#TODO_Collision: For the purpose of rendering on top of whatever object we're rendering on. Though this doesn't work in 3d spaces, will need to figure this out with a proper render fix
					debugRender = extents;
					glm::vec4 const colour = collision.isColliding ? Graphics::Colours::Red : Graphics::Colours::White;
					glm::vec3 const topLeft = glm::vec3( debugRender.cachedTopLeft, zPosMinus1 );
					glm::vec3 const topRight = glm::vec3( debugRender.cachedTopLeft.x + debugRender.cachedWidth, debugRender.cachedTopLeft.y, zPosMinus1 );
					glm::vec3 const bottomLeft = glm::vec3( debugRender.cachedBottomRight.x - debugRender.cachedWidth, debugRender.cachedBottomRight.y, zPosMinus1 );
					glm::vec3 const bottomRight = glm::vec3( debugRender.cachedBottomRight, zPosMinus1 );
					Graphics::GraphicsDraw::DrawLine( topLeft, topRight, colour, 1.f );
					Graphics::GraphicsDraw::DrawLine( topLeft, bottomLeft, colour, 1.f );
					Graphics::GraphicsDraw::DrawLine( topRight, bottomRight, colour, 1.f );
					Graphics::GraphicsDraw::DrawLine( bottomRight, bottomLeft, colour, 1.f );
				}
			} );
	}

	void CollisionDetectionSystem::CheckCollision( Core::Entity first, Core::Entity second ) const
	{
		CollisionComponent & firstCollision = m_registry.GetComponent<CollisionComponent>( first );
		CollisionComponent & secondCollision = m_registry.GetComponent<CollisionComponent>( second );

		switch ( firstCollision.Shape )
		{
			case CollisionComponent::ShapeType::Square:
			{
				switch ( secondCollision.Shape )
				{
					case CollisionComponent::ShapeType::Square:
					{
						if ( SquareSquareCollision( first, firstCollision, second, secondCollision ) )
						{
							firstCollision.isColliding = true;
							secondCollision.isColliding = true;
						}
						return;
					}
					case CollisionComponent::ShapeType::Circle:
					{
						if ( SquareCircleCollision( first, firstCollision, second, secondCollision ) )
						{
							firstCollision.isColliding = true;
							secondCollision.isColliding = true;
						}
						return;
					}
					default:
					{
						Utils::Logger::LogMessage( Utils::LogType::Error, fmt::format( "Unhandled Collision Type: {} - second entity: {}", secondCollision.Shape, second ) );
						return;
					}
				}
			}
			case CollisionComponent::ShapeType::Circle:
			{
				switch ( secondCollision.Shape )
				{
					case CollisionComponent::ShapeType::Square:
					{
						if ( SquareCircleCollision( second, secondCollision, first, firstCollision ) )
						{
							firstCollision.isColliding = true;
							secondCollision.isColliding = true;
						}
						return;
					}
					case CollisionComponent::ShapeType::Circle:
					{
						if ( CircleCircleCollision( first, firstCollision, second, secondCollision ) )
						{
							firstCollision.isColliding = true;
							secondCollision.isColliding = true;
						}
						return;
					}
					default:
					{
						Utils::Logger::LogMessage( Utils::LogType::Error, fmt::format( "Unhandled Collision Type: {} - second entity: {}", secondCollision.Shape, second ) );
						return;
					}
				}
			}
			default:
			{
				Utils::Logger::LogMessage( Utils::LogType::Error, fmt::format( "Unhandled Collision Type: {} - first entity {}", firstCollision.Shape, first ) );
				return;
			}
		}
	}

	bool CollisionDetectionSystem::SquareSquareCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const
	{
		SquareExtents const & firstExtents = m_registry.GetComponent<SquareExtents const>( first );
		//Graphics::TransformComponent const & firstTransform = m_registry.GetComponent<Graphics::TransformComponent const>( first );
		SquareExtents const & secondExtents = m_registry.GetComponent<SquareExtents const>( second );
		//Graphics::TransformComponent const & secondTransform = m_registry.GetComponent<Graphics::TransformComponent const>( second );

		/* AABB collision
		 where xy are top left coords
		 h = height
		 w = width

		 x1 + w1 >= x2
		 && x2 + w2 >= x1
		 y1 + h1 >= y2
		 && y2 + h2 >= y1

		| _w_____
		| |1    |
		| |		|
		| h	 ___|___
		| |__|__|  |
		|	 h     |
		|	 |	  2|
		|	 |_w___|
		Y
		|_X____________*/

		bool const xAxis = firstExtents.topLeft.x + firstExtents.width >= secondExtents.topLeft.x
			&& secondExtents.topLeft.x + secondExtents.width >= firstExtents.topLeft.x;
		bool const yAxis = firstExtents.topLeft.y + firstExtents.height >= secondExtents.topLeft.y
			&& secondExtents.topLeft.y + secondExtents.height >= firstExtents.topLeft.y;

		return xAxis && yAxis;
	}

	bool CollisionDetectionSystem::SquareCircleCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const
	{
		SquareExtents const & firstExtents = m_registry.GetComponent<SquareExtents const>( first );
		//Graphics::TransformComponent const & firstTransform = m_registry.GetComponent<Graphics::TransformComponent const>( first );
		CircleExtents const & secondExtents = m_registry.GetComponent<CircleExtents const>( second );
		Graphics::TransformComponent const & secondTransform = m_registry.GetComponent<Graphics::TransformComponent const>( second );

		return false;
	}

	bool CollisionDetectionSystem::CircleCircleCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const
	{
		CircleExtents const & firstExtents = m_registry.GetComponent<CircleExtents const>( first );
		Graphics::TransformComponent const & firstTransform = m_registry.GetComponent<Graphics::TransformComponent const>( first );
		CircleExtents const & secondExtents = m_registry.GetComponent<CircleExtents const>( second );
		Graphics::TransformComponent const & secondTransform = m_registry.GetComponent<Graphics::TransformComponent const>( second );

		return false;
	}

	void CollisionDetectionSystem::OnSquareExtentsConstruct( entt::registry & registry, entt::entity entity )
	{
		//Create a debug view of this extent so we can visualize
		//So the debug structure should be a mesh in which the vertices are positioned from 0,0 being the center
		/* So -
		* h = Height/2
		* w = Width /2
		* -w/h __________________w/h
		*      |		    	|
		*      |		    	|
		*      |		    	|
		*      |	  0,0	    |
		*      |		    	|
		*      |		    	|
		* -w/-h|________________|w/-h
		*/

		//Attach a debug render component
		registry.emplace<SquareExtentsDebugRenderComponent>( entity );

	}

}
