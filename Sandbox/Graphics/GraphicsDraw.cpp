#include "GraphicsDraw.h"

#include "Renderer.h"

namespace Graphics
{
	Graphics::Renderer * GraphicsDraw::m_Renderer = nullptr;

	void GraphicsDraw::Initalize( Renderer * inRenderer )
	{
		m_Renderer = inRenderer;
	}

	void GraphicsDraw::DrawLine( glm::vec3 from, glm::vec3 to, glm::vec4 colour, float thickness )
	{
		m_Renderer->DrawLine( from, to, colour, thickness );
	}

	void GraphicsDraw::DrawAALine()
	{

	}

	void GraphicsDraw::DrawCircle( glm::vec3 pos, float radius, glm::vec4 colour /*= glm::vec4( 1.f )*/, int pointCount /*= 3.f*/ )
	{
		m_Renderer->DrawCircle( pos, radius, colour, pointCount );
	}

	void GraphicsDraw::DrawWireCircle()
	{

	}

	void GraphicsDraw::DrawQuad()
	{

	}

	void GraphicsDraw::DrawWireQuad()
	{

	}
}
