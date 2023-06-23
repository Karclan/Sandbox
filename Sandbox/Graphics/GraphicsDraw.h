#pragma once

#include "Colours.h"

#include "glm/fwd.hpp"

namespace Graphics
{
	class Renderer;

	//Interface class to the renderer which exposes various ways for us to draw primiative shapes such as lines, circles, squares and other pologyons
	class GraphicsDraw
	{
	public:
		static void Initalize( Renderer * inRenderer );

		static void DrawLine(glm::vec3 from, glm::vec3 to, glm::vec4 colour = Colours::White, float thickness = 1.f );
		static void DrawAALine();

		static void DrawCircle();
		static void DrawWireCircle();

		static void DrawQuad();
		static void DrawWireQuad();

	private:
		static Renderer * m_Renderer;
	};
}
