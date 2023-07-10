#pragma once

#include "../Editor/ComponentDetails/ComponentDetailsBase.h"

namespace Physics
{
	struct CollisionComponent;
	struct SquareExtents;
	struct CircleExtents;
	struct SquareExtentsDebugRenderComponent;
	struct CircleExtentsDebugRenderComponent;
}

namespace Editor
{
	class CollisionComponentDetails : public TComponentDetails<Physics::CollisionComponent>
	{
	protected:
		void BuildDetailsInternal( Physics::CollisionComponent & inComponent ) const override;
	};

	class SquareExtentsDetails : public TComponentDetails<Physics::SquareExtents>
	{
	protected:
		void BuildDetailsInternal( Physics::SquareExtents & inComponent ) const override;
	};

	class CircleExtentsDetails : public TComponentDetails<Physics::CircleExtents>
	{
	protected:
		void BuildDetailsInternal( Physics::CircleExtents & inComponent ) const override;
	};

	class SquareExtentsDebugRenderComponentDetails : public TComponentDetails<Physics::SquareExtentsDebugRenderComponent>
	{
	protected:
		void BuildDetailsInternal( Physics::SquareExtentsDebugRenderComponent & inComponent ) const override;
	};

	class CircleExtentsDebugRenderComponentDetails : public TComponentDetails<Physics::CircleExtentsDebugRenderComponent>
	{
	protected:
		void BuildDetailsInternal( Physics::CircleExtentsDebugRenderComponent & inComponent ) const override;
	};
}
