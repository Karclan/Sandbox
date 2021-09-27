#include "ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Graphics
{
    ShaderProgram::ShaderProgram()
    {
        m_Id = glCreateProgram();
    }

    ShaderProgram::ShaderProgram( const char * shaderName, Utils::EnumBitmask<Shader::Type> flags )
        : ShaderProgram()
    {
        m_shaderName = shaderName;
        m_shaderTypeFlags = flags;
        create();
    }

    ShaderProgram::~ShaderProgram()
    {
    }

    unsigned int ShaderProgram::getId() const
    {
        return m_Id;
    }

    bool ShaderProgram::attachShader( std::vector<Shader> & shaders )
    {
        GLint success = GL_FALSE;

        if( m_shaders.empty() )
        {
            m_shaders = shaders;
        }
        else
        {
            std::cout << "Shader Warning! Attached shaders ignored as shader already has had shaders attached\n";
        }

        for( auto & shader : m_shaders )
        {
            glAttachShader( m_Id, shader.getId() );
        }
        glLinkProgram( m_Id );

        glGetProgramiv( m_Id, GL_LINK_STATUS, &success );
        if( success == GL_FALSE )
        {
            int infoLogSize = 0;
            glGetProgramiv( m_Id, GL_INFO_LOG_LENGTH, &infoLogSize );
            std::vector<GLchar> infoLogVec( infoLogSize );
            if( !infoLogVec.empty() )
            {

                glGetProgramInfoLog( m_Id, infoLogSize, NULL, &infoLogVec[ 0 ] );
                std::string infoLog( infoLogVec.begin(), infoLogVec.end() );
                std::cout << "Error::Shader::Program::Linking_Failed\n" << infoLog << std::endl;
            }
            else
            {
                std::cout << "Error::Shader::Program::Linking_Failed\n" << "No error log output!" << std::endl;
            }
        }

        for( auto & shader : m_shaders )
        {
            glDeleteShader( shader.getId() );
        }


        return success == GL_TRUE;
    }

    bool ShaderProgram::create()
    {
        std::vector<Shader> shaders;
        
        size_t setBits = m_shaderTypeFlags.count();
        
        for( size_t i = 0; i < setBits; ++i )
        {
            if( m_shaderTypeFlags.test( i ) )
            {
                Shader::Type t = static_cast<Shader::Type>(i);
                if( t < Shader::Type::Total && t != Shader::Type::Invalid )
                {
                    shaders.push_back( { t, m_shaderName.c_str() } );
                }
            }
        }

        return attachShader(shaders);
    }

    void ShaderProgram::useProgram() const
    {
        glUseProgram( m_Id );
    }

    void ShaderProgram::setUniform( const char * name, float x, float y, float z )
    {
        GLint loc = getUniformLocation( name );
        glUniform3f( loc, x, y, z );
    }

    void ShaderProgram::setUniform( const char * name, glm::vec3 const & v )
    {
        this->setUniform( name, v.x, v.y, v.z );
    }

    void ShaderProgram::setUniform( const char * name, glm::vec4 const & v )
    {
        GLint loc = getUniformLocation( name );
        glUniform4f( loc, v.x, v.y, v.z, v.w );
    }

    void ShaderProgram::setUniform( const char * name, glm::vec2 const & v )
    {
        GLint loc = getUniformLocation( name );
        glUniform2f( loc, v.x, v.y );
    }

    void ShaderProgram::setUniform( const char * name, glm::mat4 const & m )
    {
        GLint loc = getUniformLocation( name );
        glUniformMatrix4fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
    }

    void ShaderProgram::setUniform( const char * name, glm::mat3 const & m )
    {
        GLint loc = getUniformLocation( name );
        glUniformMatrix3fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
    }

    void ShaderProgram::setUniform( const char * name, float val )
    {
        GLint loc = getUniformLocation( name );
        glUniform1f( loc, val );
    }

    void ShaderProgram::setUniform( const char * name, int val )
    {
        GLint loc = getUniformLocation( name );
        glUniform1i( loc, val );
    }

    void ShaderProgram::setUniform( const char * name, GLuint val )
    {
        GLint loc = getUniformLocation( name );
        glUniform1ui( loc, val );
    }

    void ShaderProgram::setUniform( const char * name, bool val )
    {
        int loc = getUniformLocation( name );
        glUniform1i( loc, val );
    }

    void ShaderProgram::printActiveUniforms() const
    {
        GLint numUniforms = 0;
        glGetProgramInterfaceiv( m_Id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms );

        GLenum properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

        printf( "Active uniforms:\n" );
        for( int i = 0; i < numUniforms; ++i )
        {
            GLint results[ 4 ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM, i, 4, properties, 4, NULL, results );

            if( results[ 3 ] != -1 )  // Skip uniforms in blocks
            {
                continue;
            }

            GLint nameBufSize = results[ 0 ] + 1;
            char * name = new char[ nameBufSize ];
            glGetProgramResourceName( m_Id, GL_UNIFORM, i, nameBufSize, NULL, name );
            printf( "%-5d %s (%s)\n", results[ 2 ], name, getTypeString( results[ 1 ] ) );
            delete [] name;
        }
    }

    void ShaderProgram::printActiveUniformBlocks() const
    {
        GLint numBlocks = 0;
        glGetProgramInterfaceiv( m_Id, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks );
        GLenum blockProps [] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
        GLenum blockIndex [] = { GL_ACTIVE_VARIABLES };
        GLenum props [] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

        for( int block = 0; block < numBlocks; ++block )
        {
            GLint blockInfo[ 2 ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo );

            GLint numUnis = blockInfo[ 0 ];
            char * blockName = new char[ blockInfo[ 1 ] + 1 ];
            glGetProgramResourceName( m_Id, GL_UNIFORM_BLOCK, block, blockInfo[ 1 ] + 1, NULL, blockName );

            printf( "Uniform block \"%s\":\n", blockName );
            delete [] blockName;

            GLint * unifIndexes = new GLint[ numUnis ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes );

            for( int unif = 0; unif < numUnis; ++unif )
            {
                GLint uniIndex = unifIndexes[ unif ];
                GLint results[ 3 ];
                glGetProgramResourceiv( m_Id, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results );

                GLint nameBufSize = results[ 0 ] + 1;
                char * name = new char[ nameBufSize ];
                glGetProgramResourceName( m_Id, GL_UNIFORM, uniIndex, nameBufSize, NULL, name );
                printf( "    %s (%s)\n", name, getTypeString( results[ 1 ] ) );
                delete [] name;
            }

            delete [] unifIndexes;
        }
    }

    void ShaderProgram::printActiveAttribs() const
    {
        GLint numAttribs;
        glGetProgramInterfaceiv( m_Id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs );

        GLenum properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

        printf( "Active attributes:\n" );
        for( int i = 0; i < numAttribs; ++i )
        {
            GLint results[ 3 ];
            glGetProgramResourceiv( m_Id, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results );

            GLint nameBufSize = results[ 0 ] + 1;
            char * name = new char[ nameBufSize ];
            glGetProgramResourceName( m_Id, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name );
            printf( "%-5d %s (%s)\n", results[ 2 ], name, getTypeString( results[ 1 ] ) );
            delete [] name;
        }
    }

    GLint ShaderProgram::getUniformLocation( const char * name )
    {
        UniformMap::const_iterator pos;
        pos = uniformLocations.find( name );

        if( pos == uniformLocations.end() )
        {
            int loc = glGetUniformLocation( m_Id, name );
            uniformLocations[ name ] = loc;
            if( loc == -1 )
            {
                printf( "Unable to find Uniform Location: %s. Check the %s shader!\n", name,  m_shaderName.c_str() );
            }
        }

        return uniformLocations[ name ];
    }

    const char * ShaderProgram::getTypeString( GLenum type ) const
    {
        switch( type )
        {
        case GL_FLOAT:
            return "float";
        case GL_FLOAT_VEC2:
            return "vec2f";
        case GL_FLOAT_VEC3:
            return "vec3f";
        case GL_FLOAT_VEC4:
            return "vec4f";
        case GL_DOUBLE:
            return "double";
        case GL_INT:
            return "int";
        case GL_UNSIGNED_INT:
            return "unsigned int";
        case GL_BOOL:
            return "bool";
        case GL_FLOAT_MAT2:
            return "mat2f";
        case GL_FLOAT_MAT3:
            return "mat3f";
        case GL_FLOAT_MAT4:
            return "mat4f";
        default:
            return "?";
        }
    }
}