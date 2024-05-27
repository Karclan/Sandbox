#include "GraphicsDraw.h"

#include "Renderer.h"

#include "ShaderComponent.h"
#include "TextureComponent.h"
#include "MeshComponent.h"

#include "../Core/ResourceManager.h"


namespace Graphics
{
	Graphics::Renderer * GraphicsDraw::m_Renderer = nullptr;

	void GraphicsDraw::Initalize( Renderer * inRenderer )
	{
		m_Renderer = inRenderer;
	}

	void GraphicsDraw::DrawWorldLine( glm::vec3 from, glm::vec3 to, glm::vec4 colour, float thickness )
	{
		m_Renderer->DrawWorldLine( from, to, colour, thickness );
	}

	void GraphicsDraw::DrawScreenLine( glm::vec2 from, glm::vec3 to, glm::vec4 colour /*= Colours::White*/, float thickness /*= 1.f */ )
	{
		m_Renderer->DrawScreenLine( from, to, colour, thickness );
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

	void GraphicsDraw::DrawWireQuad( glm::vec3 pos, glm::vec2 dimensons, glm::vec4 colour, float thickness )
	{
		glm::vec2 const halfDimensons { dimensons.x / 2.f , dimensons.y / 2.f };

		glm::vec3 const topLeft { pos.x - halfDimensons.x,
								  pos.y + halfDimensons.y,
								  pos.z };

		glm::vec3 const topRight { pos.x + halfDimensons.x,
								   pos.y + halfDimensons.y,
								   pos.z };

		glm::vec3 const bottomLeft { pos.x - halfDimensons.x,
									 pos.y - halfDimensons.y,
									 pos.z };
		glm::vec3 const bottomRight { pos.x + halfDimensons.x,
									  pos.y - halfDimensons.y,
									  pos.z };

		m_Renderer->DrawWorldLine( topLeft, topRight, colour, 1.f );
		m_Renderer->DrawWorldLine( topLeft, bottomLeft, colour, 1.f );
		m_Renderer->DrawWorldLine( topRight, bottomRight, colour, 1.f );
		m_Renderer->DrawWorldLine( bottomRight, bottomLeft, colour, 1.f );
	}

	void GraphicsDraw::DrawWorldPoint( glm::vec3 pos, float size /*= 1.f*/, glm::vec4 colour /*= Colours::White */ )
	{
		m_Renderer->DrawWorldPoint( pos, size, colour );
	}

	void GraphicsDraw::DrawScreenPoint( glm::vec2 pos, float size /*= 1.f*/, glm::vec4 colour /*= Colours::White */ )
	{
		m_Renderer->DrawScreenPoint( pos, size, colour );
	}

	Core::Entity GraphicsDraw::CreateSprite( Core::Registry & registry, char const * name, char const * texture )
	{
		Core::Entity const entity = registry.CreateEntity( name );
		registry.AddComponent<Graphics::MeshComponent>( entity,
			Core::Resources::CreateOrLoadMesh( "Square" )
			);

		registry.AddComponent<Graphics::ShaderComponent>( entity,
			Core::Resources::CreateOrLoadShader( "MainShader" )
			);

		registry.AddComponent<Graphics::TextureComponent>( entity,
			Core::Resources::CreateOrLoadTexture( { texture } )
			);

		return entity;
	}

}
