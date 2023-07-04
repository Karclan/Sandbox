#include "OpenGLVertexArrayObject.h"

#include "../Utils/DebugBreak.h"
#include "../Utils/Assert.h"

#include "glad/glad.h"

namespace Graphics
{
	static GLenum ShaderDataTypeToOpenGLBaseType( ShaderDataType type )
	{
		switch ( type )
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		SB_DEBUGBREAK();
		return 0;
	}


	OpenGLVertexArrayObject::OpenGLVertexArrayObject()
	{
		glCreateVertexArrays( 1, &m_Id );
	}

	OpenGLVertexArrayObject::~OpenGLVertexArrayObject()
	{
		glDeleteVertexArrays( 1, &m_Id );
	}

	void OpenGLVertexArrayObject::Bind()
	{
		glBindVertexArray( m_Id );
	}

	void OpenGLVertexArrayObject::UnBind()
	{
		glBindVertexArray( 0 );
	}

	void OpenGLVertexArrayObject::AddVertexBuffer( std::shared_ptr<VertexBuffer> const & vertexBuffer )
	{
		SB_CORE_ASSERT( vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout" );

		glBindVertexArray( m_Id );
		vertexBuffer->Bind();

		const auto & layout = vertexBuffer->GetLayout();
		for ( const auto & element : layout )
		{
			ShaderDataType const type = element.ElementType;
			uint32_t const componentCount = GetComponentCount( type );
			GLenum const OpenGLBaseType = ShaderDataTypeToOpenGLBaseType( type );
			uint32_t const stride = layout.GetStride();

			switch ( type )
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray( m_VertexBufferIndex );
					glVertexAttribPointer( m_VertexBufferIndex,
						componentCount,
						OpenGLBaseType,
						element.Normalized ? GL_TRUE : GL_FALSE,
						stride,
						( const void * ) element.Offset );
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray( m_VertexBufferIndex );
					glVertexAttribIPointer( m_VertexBufferIndex,
						componentCount,
						OpenGLBaseType,
						stride,
						( const void * ) element.Offset );
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					for ( uint8_t i = 0; i < componentCount; i++ )
					{
						glEnableVertexAttribArray( m_VertexBufferIndex );
						glVertexAttribPointer( m_VertexBufferIndex,
							componentCount,
							OpenGLBaseType,
							element.Normalized ? GL_TRUE : GL_FALSE,
							stride,
							( const void * ) ( element.Offset + sizeof( float ) * componentCount * i ) );
						glVertexAttribDivisor( m_VertexBufferIndex, 1 );
						m_VertexBufferIndex++;
					}
					break;
				}
				default:
					SB_CORE_ASSERT( false, "Unknown ShaderDataType" );
					//HZ_CORE_ASSERT( false, "Unknown ShaderDataType!" );
			}
		}

		m_VertexBuffers.push_back( vertexBuffer );
	}

	void OpenGLVertexArrayObject::SetIndexBuffer( std::shared_ptr<IndexBuffer> const & indexBuffer )
	{
		glBindVertexArray( m_Id );
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
