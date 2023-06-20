#pragma once

#include "glm/glm.hpp"

namespace Game
{
	struct GameGridComponent
	{
		int Height { 10 };
		int Width { 8 };

		glm::vec3 TopLeft { 0.f };
		glm::vec3 BottomRight { 0.f };

		bool DimensionsChanged { true };

		//Debug rendering flag to draw points for the size of the grid
		glm::vec4 DebugColour = glm::vec4 { 0.f, 1.f, 0.f, 1.f };
		bool Visualize { true };
	};
}
