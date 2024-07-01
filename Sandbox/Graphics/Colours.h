#pragma once

#include "glm/ext/vector_float4.hpp"
#include <glm/gtx/color_space.hpp>

namespace Graphics
{
	struct Colours
	{
		static constexpr glm::vec4 Red { 1.f, 0.f, 0.f, 1.f };
		static constexpr glm::vec4 Green { 0.f, 1.f, 0.f, 1.f };
		static constexpr glm::vec4 Blue { 0.f, 0.f, 1.f, 1.f };
		static constexpr glm::vec4 Black { 0.f, 0.f, 0.f, 1.f };
		static constexpr glm::vec4 White { 1.f, 1.f, 1.f, 1.f };
		static constexpr glm::vec4 Colour( float r, float g, float b, float a ) { return glm::vec4( r, g, b, a ); }
	};
}
