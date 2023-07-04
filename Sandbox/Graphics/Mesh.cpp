#include "Mesh.h"

#include "../Utils/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
	Mesh::~Mesh()
	{
		CleanUp();
	}

	void Mesh::Bind()
	{
		m_VAO->Bind();
	}

	void Mesh::CleanUp()
	{
		if ( m_Created )
		{
			m_VAO.reset();
			m_VBO.reset();
			m_EBO.reset();
			m_Created = false;
		}
	}

	void Mesh::FinishBinding()
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );
	}

	void Mesh::SetCreated( bool flag )
	{
		if ( flag && !m_Created )
		{
			if ( m_VAO )
			{
				m_Created = flag;
			}
			else //Not successfully generated and bound expected buffers
			{
				Utils::Logger::LogMessage( Utils::LogType::Error, "Mesh Creation Error!, trying to set created when buffers aren't made." );
			}
		}
		else if ( !flag && m_Created ) //Improper use of setCreated
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, "Mesh Creation Error!, trying to set uncreated when created, use cleanup!" );
		}
		else //flag and created
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, "Mesh Creation Error!, trying to set created when already created." );
		}
	}

	bool Mesh::GetCreated() const
	{
		return m_Created;
	}
}

