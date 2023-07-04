#include "Square.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
	Square::Square()
	{
		Create();
	}

	void Square::Create()
	{
		if ( !GetCreated() )
		{
			static constexpr float vertices[] = {
				/*Vertex*/  0.5f,  0.5f, 0.0f, /*Normal*/ 0.f, 0.f, 1.f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 1.0f, 1.0f, // top right
				/*Vertex*/  0.5f, -0.5f, 0.0f, /*Normal*/ 0.f, 0.f, 1.f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 1.0f, 0.0f, // bottom right
				/*Vertex*/ -0.5f, -0.5f, 0.0f, /*Normal*/ 0.f, 0.f, 1.f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 0.0f, 0.0f, // bottom left
				/*Vertex*/ -0.5f,  0.5f, 0.0f, /*Normal*/ 0.f, 0.f, 1.f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 0.0f, 1.0f, // top left 
			};
			
			static constexpr unsigned int indices[] = {
				 0, 1, 3,   // first triangle
				 1, 2, 3    // second triangle
			};

			m_VAO = VertexArrayObject::Create();
			m_VAO->Bind();
			m_VBO = VertexBuffer::Create( vertices, sizeof( vertices ) );
			m_VBO->SetLayout( { 
				BufferElement::Create( ShaderDataType::Float3, "aPos" ),
				BufferElement::Create( ShaderDataType::Float3, "aNormal" ),
				BufferElement::Create( ShaderDataType::Float3, "aColor" ),
				BufferElement::Create( ShaderDataType::Float2, "ATexCoord"),
				} );
			m_VAO->AddVertexBuffer( m_VBO );
			m_EBO = IndexBuffer::Create( indices, sizeof( indices ) );
			m_VAO->SetIndexBuffer( m_EBO );

			FinishBinding();
			SetCreated( true );
		}
	}
}
