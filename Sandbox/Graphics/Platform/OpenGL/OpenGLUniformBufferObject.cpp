#include "OpenGLUniformBufferObject.h"

#include "glad/glad.h"

namespace Graphics
{
	OpenGLUniformBufferObject::OpenGLUniformBufferObject( uint32_t size, uint32_t binding )
	{
		glCreateBuffers( 1, &m_Id );
		glNamedBufferData( m_Id, size, nullptr, GL_DYNAMIC_DRAW ); // TODO: investigate usage hint
		glBindBufferBase( GL_UNIFORM_BUFFER, binding, m_Id );
	}

	OpenGLUniformBufferObject::~OpenGLUniformBufferObject()
	{
		glDeleteBuffers( 1, &m_Id );
	}

	void OpenGLUniformBufferObject::SetData( void const * data, uint32_t size, uint32_t offset /*= 0 */ )
	{
		glNamedBufferSubData( m_Id, offset, size, data );
	}

	void OpenGLUniformBufferObject::Bind()
	{
		
	}

	void OpenGLUniformBufferObject::UnBind()
	{
		
	}
}
