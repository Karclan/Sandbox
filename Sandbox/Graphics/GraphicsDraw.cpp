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

	void GraphicsDraw::DrawCircle()
	{

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
