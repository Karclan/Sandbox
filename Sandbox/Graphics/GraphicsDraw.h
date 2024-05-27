#pragma once

#include "Colours.h"

#include "../Core/CoreRegistry.h"

#include "glm/fwd.hpp"

namespace Graphics
{
	class Renderer;

	//Interface class to the renderer which exposes various ways for us to draw primiative shapes such as lines, circles, squares and other pologyons
	class GraphicsDraw
	{
	public:
		static void Initalize( Renderer * inRenderer );

		static void DrawWorldLine( glm::vec3 from, glm::vec3 to, glm::vec4 colour = Colours::White, float thickness = 1.f );
		static void DrawScreenLine( glm::vec2 from, glm::vec3 to, glm::vec4 colour = Colours::White, float thickness = 1.f );
		static void DrawAALine();

		static void DrawCircle( glm::vec3 pos, float radius, glm::vec4 colour = Colours::White, int pointCount = 3.f );
		static void DrawWireCircle();

		static void DrawQuad();
		static void DrawWireQuad( glm::vec3 pos, glm::vec2 dimensons, glm::vec4 colour = Colours::White, float thickness = 1.f );

		static void DrawWorldPoint( glm::vec3 pos, float size = 1.f, glm::vec4 colour = Colours::White );
		static void DrawScreenPoint( glm::vec2 pos, float size = 1.f, glm::vec4 colour = Colours::White );

		static Core::Entity CreateSprite( Core::Registry & registry, char const * name, char const * texture );

	private:
		static Renderer * m_Renderer;
	};
}
