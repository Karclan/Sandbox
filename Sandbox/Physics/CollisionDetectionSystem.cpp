#include "CollisionDetectionSystem.h"
#include "CollisionComponent.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"
#include "../Utils/customformatters.h"
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
		m_registry.on_construct<CircleExtents>().connect<&CollisionDetectionSystem::OnCircleExtentsConstruct>( this );
	}

	CollisionDetectionSystem::~CollisionDetectionSystem()
	{
		m_registry.on_construct<SquareExtents>().disconnect<&CollisionDetectionSystem::OnSquareExtentsConstruct>( this );
		m_registry.on_construct<CircleExtents>().disconnect<&CollisionDetectionSystem::OnCircleExtentsConstruct>( this );
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
					Graphics::GraphicsDraw::DrawWorldLine( topLeft, topRight, colour, 1.f );
					Graphics::GraphicsDraw::DrawWorldLine( topLeft, bottomLeft, colour, 1.f );
					Graphics::GraphicsDraw::DrawWorldLine( topRight, bottomRight, colour, 1.f );
					Graphics::GraphicsDraw::DrawWorldLine( bottomRight, bottomLeft, colour, 1.f );
				}
			} );

		m_registry.ForView<CollisionComponent const, CircleExtents const, CircleExtentsDebugRenderComponent, Graphics::TransformComponent const>( [] ( CollisionComponent const & collision, CircleExtents const & extents, CircleExtentsDebugRenderComponent & debugRender, Graphics::TransformComponent const & transform )
			{
				if ( collision.drawDebugLines )
				{
					glm::vec3 const zPosMinus1 = transform.Position + glm::vec3( 0.f, 0.f, -1.f ); //#TODO_Collision: For the purpose of rendering on top of whatever object we're rendering on. Though this doesn't work in 3d spaces, will need to figure this out with a proper render fix
					debugRender = extents;
					glm::vec4 const colour = collision.isColliding ? Graphics::Colours::Red : Graphics::Colours::White;
					Graphics::GraphicsDraw::DrawCircle( zPosMinus1, debugRender.CachedRadius, colour, debugRender.PointCount );
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
		SquareExtents const & squareExtents = m_registry.GetComponent<SquareExtents const>( first );
		Graphics::TransformComponent const & firstTransform = m_registry.GetComponent<Graphics::TransformComponent const>( first );
		CircleExtents const & circleExtents = m_registry.GetComponent<CircleExtents const>( second );
		Graphics::TransformComponent const & secondTransform = m_registry.GetComponent<Graphics::TransformComponent const>( second );

		/*
		* Taken from https://gamedev.stackexchange.com/questions/179635/circle-square-collision-detection-simple-formula
		* We can think of collisions between a square and a circle as a collision between a rounded square and a point.
		* Effectively, we strip off all the "round stuff" making up the disc, 
		* and add it onto the square instead (an operation called Minkowski addition).
		* 
		*		        * * * *
		*		     *			  *
		*		   *				*
		*		  *					 *
		*		  *	       ()		 *
		*		  *					 *
		*			*				*
		*		     *			  *
		*		        * * * *
		*  _________
		* |			|
		* |			|
		* |			|
		* |_________|
		* 
		* //"rounded circle in ascii lol
		*				()
		*	***********	     			    
		* *  _________	*
		* *	|		  | *
		* *	|		  | *
		* *	|		  | *
		* *	|_________| *
		* *				*
		*	***********
		* 
		* If the the remaining center point of the circle is inside the rounded square, 
		* then the original square and circle collide. If there's a separation between the point and the rounded square, 
		* then the original square and circle are separated by this same distance.
		* 
		* We can quickly compute the signed distance of a point from a rounded rectangle 
		* (a square is just a special rectangle with equal width and height)
		* https://iquilezles.org/articles/distfunctions2d/
		*/

		glm::vec2 const relativeCenter = secondTransform.Position - firstTransform.Position;
		glm::vec2 const halfRectangleExtents = glm::vec2( squareExtents.width / 2, squareExtents.height / 2 );
		glm::vec2 const offsetFromCorner = glm::abs( relativeCenter ) - halfRectangleExtents;
		float const offsetLength = glm::length( glm::max( offsetFromCorner, glm::vec2( 0.f ) ) );

		/*
		* If this value is negative, the circle and square overlap by this penetration distance. 
		* If it's positive, they're separated by this distance. 
		* If it's zero, they're kissing exactly edge-to-edge / edge-to-corner.
		*/
		float const intersectionDistance = glm::min( glm::max( offsetFromCorner.x, offsetFromCorner.y ), 0.f )
										   + offsetLength
										   - circleExtents.radius;

		return intersectionDistance<=0;
	}

	bool CollisionDetectionSystem::CircleCircleCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const
	{
		CircleExtents const & firstExtents = m_registry.GetComponent<CircleExtents const>( first );
		Graphics::TransformComponent const & firstTransform = m_registry.GetComponent<Graphics::TransformComponent const>( first );
		CircleExtents const & secondExtents = m_registry.GetComponent<CircleExtents const>( second );
		Graphics::TransformComponent const & secondTransform = m_registry.GetComponent<Graphics::TransformComponent const>( second );

		glm::vec3 const SecondFirstVec = firstTransform.Position - secondTransform.Position;
		float const RadiusSum = firstExtents.radius + secondExtents.radius;
		float const RadiusSumSqrd = RadiusSum * RadiusSum;
		float magSquared = glm::dot( SecondFirstVec, SecondFirstVec );

		bool const intersecting = magSquared < RadiusSumSqrd;

		return intersecting;
	}

	void CollisionDetectionSystem::OnSquareExtentsConstruct( entt::registry & registry, entt::entity entity )
	{
		CollisionComponent & collisionComponent = registry.get_or_emplace<CollisionComponent>( entity );
		collisionComponent.Shape = CollisionComponent::ShapeType::Square;

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

	void CollisionDetectionSystem::OnCircleExtentsConstruct( entt::registry & registry, entt::entity entity )
	{
		CollisionComponent & collisionComponent = registry.get_or_emplace<CollisionComponent>( entity );
		collisionComponent.Shape = CollisionComponent::ShapeType::Circle;

		//Create a debug view of this so we can visualize
		//We have circle drawing within the renderer so all we need to do set a radius and a point count (for accuracy of the circle shape)

		/* Currently the way we render the circles is through the geometry shader
		*  We specify the number of points we want to represent the circle, and the radius
		*  so we then go through 2pi in intervals of 2pi / point count
		*  then we scale these new positions by our radius amount
		*
		*        * * * *
		*     *		^	  *
		*   *		|		*
		*  *		R		 *
		*  *<----R|0,0|R---->*
		*  *		R		 *
		*	*		|		*
		*     *		V	  *
		*        * * * *
		*/
		registry.emplace<CircleExtentsDebugRenderComponent>( entity );
	}

}
