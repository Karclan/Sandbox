#include "OpenGLDataBuffer.h"

#include <glad/glad.h>

namespace Graphics
{



	OpenGLVertexBuffer::OpenGLVertexBuffer( uint32_t dataSize )
	{
		m_dataSize = dataSize;
		CreateBindAndSetBuffer( m_Id, m_dataSize, nullptr, GL_DYNAMIC_DRAW );
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer( float * vertices, uint32_t dataSize )
	{
		m_dataSize = dataSize;
		CreateBindAndSetBuffer( m_Id, m_dataSize, vertices, GL_STATIC_DRAW );
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer( float const * vertices, uint32_t dataSize )
	{
		m_dataSize = dataSize;
		CreateBindAndSetBuffer( m_Id, m_dataSize, vertices, GL_STATIC_DRAW );
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers( 1, &m_Id );
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_Id );
	}

	void OpenGLVertexBuffer::UnBind()
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void OpenGLVertexBuffer::CreateBindAndSetBuffer( uint32_t & id, uint32_t size, void const * vertices, auto drawType )
	{
		glCreateBuffers( 1, &id );
		Bind();
		glBufferData( GL_ARRAY_BUFFER, size, vertices, drawType );
	}

	void OpenGLVertexBuffer::SetData( const void * data, uint32_t dataSize )
	{
		m_dataSize = dataSize;
		Bind();
		glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, data );
	}


	void OpenGLVertexBuffer::SetLayout( BufferLayout const & layout )
	{
		m_Layout = layout;

		uint32_t stride = m_Layout.GetStride();
		m_dataSize;
		m_vertexCount = m_dataSize / stride;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer( uint32_t * indicies, uint32_t count )
		: m_Count( count )
	{
		glCreateBuffers( 1, &m_Id );
		Bind();

		//Notes from Cherno's Hazel engine implementation. I'd like to change this then so we're scoped inside of a bound VAO

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
		//glBufferData( GL_ARRAY_BUFFER, count * sizeof( uint32_t ), indicies, GL_STATIC_DRAW );

		//Intention is to have some sort of scoped binding match so we have to have a bound VAO when we're binding indicies
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( uint32_t ), indicies, GL_STATIC_DRAW );
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer( uint32_t const * indicies, uint32_t count )
		: m_Count( count )
	{
		glCreateBuffers( 1, &m_Id );
		Bind();

		//Notes from Cherno's Hazel engine implementation. I'd like to change this then so we're scoped inside of a bound VAO

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
		//glBufferData( GL_ARRAY_BUFFER, count * sizeof( uint32_t ), indicies, GL_STATIC_DRAW );

		//Intention is to have some sort of scoped binding match so we have to have a bound VAO when we're binding indicies
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( uint32_t ), indicies, GL_STATIC_DRAW );
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers( 1, &m_Id );
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Id );
	}

	void OpenGLIndexBuffer::UnBind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

}
