#pragma once

#include "glm/glm.hpp"

#include <cstdint>

namespace Physics
{
	//We structure collisions into a series of components
	//A collision component defines the type
	//Then from the type we can grab the extents component
	//Currently we're just doing simple AABB collisions and circle to square collisions
	//In the future there will be a swept collision implementation

	struct CollisionComponent
	{
		enum class ShapeType : int8_t
		{
			Circle,
			Square
		};
		ShapeType Shape { ShapeType::Square };

		bool isColliding { false };
		bool drawDebugLines { false };
	};

	struct SquareExtents
	{
		float width { 0.f };
		float height { 0.f };

		bool recalculateExtents { true };

		glm::vec2 topLeft { 0.f };
		glm::vec2 bottomRight { 0.f };
		glm::vec2 offset { 0.f };
	};

	struct CircleExtents
	{
		float radius { 0.f };
	};

	struct SquareExtentsDebugRenderComponent
	{
		SquareExtentsDebugRenderComponent operator=( SquareExtents const & extents )
		{
			cachedTopLeft = extents.topLeft;
			cachedBottomRight = extents.bottomRight;
			cachedWidth = extents.width;
			cachedHeight = extents.height;
			cachedOffset = extents.offset;
			return *this;
		}

		bool operator==( SquareExtents const & extents ) const
		{
			return ( 
				   ( cachedTopLeft == extents.topLeft )
				|| ( cachedTopLeft == extents.bottomRight )
				|| ( cachedOffset == extents.offset )
				|| ( cachedWidth == extents.width )
				|| ( cachedHeight == extents.height ) 
				);
		}

		bool operator!=( SquareExtents const & extents ) const
		{
			return !( *this == extents );
		}

		float cachedWidth { 0.f };
		float cachedHeight { 0.f };

		glm::vec2 cachedTopLeft { 0.f };
		glm::vec2 cachedBottomRight { 0.f };
		glm::vec2 cachedOffset { 0.f };

		bool recalculateDebugMesh { false };
	};

	struct CircleExtentsDebugRenderComponent
	{
		CircleExtentsDebugRenderComponent operator=( CircleExtents const & extents )
		{
			CachedRadius = extents.radius;
			return *this;
		}

		bool operator==( CircleExtents const & extents ) const
		{
			return (CachedRadius == extents.radius );
		}

		bool operator!=( CircleExtents const & extents ) const
		{
			return !( *this == extents );
		}

		float CachedRadius { 0.f };
		int PointCount { 24 };
	};
}
