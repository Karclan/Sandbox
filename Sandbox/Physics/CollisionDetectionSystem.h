#pragma once

#include "../Core/Subsystem.h"

namespace Graphics
{
	class Window;
}

namespace Physics
{
	struct CollisionComponent;

	class CollisionDetectionSystem : public Core::Subsystem
	{
	public:
		CollisionDetectionSystem(Core::Registry & registry, Graphics::Window & window );
		virtual ~CollisionDetectionSystem();
		CollisionDetectionSystem( CollisionDetectionSystem const & other ) = delete;
		CollisionDetectionSystem( CollisionDetectionSystem && other ) = delete;
		CollisionDetectionSystem & operator=( CollisionDetectionSystem const & other ) = delete;
		CollisionDetectionSystem & operator=( CollisionDetectionSystem && other ) = delete;

		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;

	private:
		void CheckCollision( Core::Entity first, Core::Entity second ) const;

		bool SquareSquareCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const;
		bool SquareCircleCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const;
		bool CircleCircleCollision( Core::Entity first, CollisionComponent & firstCollision, Core::Entity second, CollisionComponent & secondCollision ) const;

		void OnSquareExtentsConstruct(entt::registry & registry, entt::entity entity);

		Graphics::Window & m_window;
	};
}
