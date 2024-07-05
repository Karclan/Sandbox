#pragma once
#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"

namespace Game
{
	struct FallingSandStateComponent
	{
		glm::vec2 Gravity;
		glm::ivec2 GridStartPosition = {0,0}; //The positions of the grid, so we can arbitrarily move it around our world 
		static unsigned int const CellSize = 1u; //The actual size of the cells
		static unsigned int const GridSize = 500u;
		static unsigned int const RowsLength= GridSize;
		static unsigned int const ColumnsLength= GridSize;

		float GridSandStateArray[GridSize][GridSize] = { 0 };
	};
}
