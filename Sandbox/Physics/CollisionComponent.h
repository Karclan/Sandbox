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
	};

	struct SquareExtents
	{
		double width { 0.f };
		double height { 0.f };

		bool recalculateExtents { true };

		glm::vec2 topLeft { 0.f };
		glm::vec2 bottomRight { 0.f };
	};

	struct CircleExtents
	{
		double radius { 0.f };
	};

	struct SquareExtentsDebugRenderComponent
	{
		SquareExtentsDebugRenderComponent operator=( SquareExtents const & extents )
		{
			cachedTopLeft = extents.topLeft;
			cachedBottomRight = extents.bottomRight;
			cachedWidth = extents.width;
			cachedHeight = extents.height;
			return *this;
		}

		bool operator==( SquareExtents const & extents ) const
		{
			return ( 
				   ( cachedTopLeft == extents.topLeft )
				|| ( cachedTopLeft == extents.bottomRight )
				|| ( cachedWidth == extents.width )
				|| ( cachedHeight == extents.height ) 
				);
		}

		bool operator!=( SquareExtents const & extents ) const
		{
			return !( *this == extents );
		}

		double cachedWidth { 0.f };
		double cachedHeight { 0.f };

		glm::vec2 cachedTopLeft { 0.f };
		glm::vec2 cachedBottomRight { 0.f };
		bool recalculateDebugMesh { false };
	};
}
