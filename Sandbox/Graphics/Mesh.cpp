#include "Mesh.h"

#include "../Utils/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
        cleanUp();
    }

    void Mesh::draw() const
    {
        glBindVertexArray( m_vao );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    }

    void Mesh::bind()
    {
        glBindVertexArray( m_vao );
    }

    void Mesh::cleanUp()
    {
        if (m_created)
        {
            glDeleteVertexArrays( 1, &m_vao );
            glDeleteBuffers( 1, &m_vbo );
            glDeleteBuffers( 1, &m_ebo );
            m_created = false;
        }
    }

    void Mesh::bindVAO()
    {
        if (m_vao == 0)
        {
            glGenVertexArrays( 1, &m_vao );
        }

        glBindVertexArray( m_vao );
    }

    void Mesh::bindVertices( float const * const vertices, unsigned int size, unsigned int stride )
    {
        if (m_vbo == 0)
        {
            glGenBuffers( 1, &m_vbo );
        }
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

        //Vertex points
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride * sizeof( float ), ( void * ) 0 );
        glEnableVertexAttribArray( 0 );

        //Vertex Colour
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride * sizeof( float ), ( void * ) (3 * sizeof( float )) );
        glEnableVertexAttribArray( 1 );

        //Vertex Texture Coords
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, stride * sizeof( float ), ( void * ) ( 6 * sizeof( float ) ) );
        glEnableVertexAttribArray( 2 );
    }

    void Mesh::bindIndices( unsigned int const * const indices, unsigned int size )
    {
        m_hasIndicies = true;
        if (m_ebo == 0)
        {
            glGenBuffers( 1, &m_ebo );
        }
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
    }

    void Mesh::finishBinding()
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindVertexArray( 0 );
    }

    void Mesh::setCreated( bool flag )
    {
        if (flag && !m_created)
        {
            if (( m_ebo != 0 || !m_hasIndicies ) &&
                m_vao != 0 &&
                m_vbo != 0)
            {
                m_created = flag;
            }
            else //Not successfully generated and bound expected buffers
            {
                Utils::Logger::LogMessage(Utils::LogType::Error, "Mesh Creation Error!, trying to set created when buffers aren't made.");
            }
        }
        else if (!flag && m_created) //Improper use of setCreated
        {
            Utils::Logger::LogMessage(Utils::LogType::Error, "Mesh Creation Error!, trying to set uncreated when created, use cleanup!");
        }
        else //flag and created
        {
            Utils::Logger::LogMessage(Utils::LogType::Error, "Mesh Creation Error!, trying to set created when already created.");
        }
    }

    bool Mesh::getCreated() const
    {
        return m_created;
    }
}
