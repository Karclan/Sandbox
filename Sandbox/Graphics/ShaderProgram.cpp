#include "ShaderProgram.h"

#include "../Utils/Logger.h"
#include "../Utils/format.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Graphics
{
    ShaderProgram::ShaderProgram( std::string const & shaderName, Utils::EnumBitmask<Shader::Type> flags )
        : m_shaderName(shaderName)
        , m_shaderTypeFlags(flags)
    {
        m_Id = glCreateProgram();
        create();

        /*m_boolUniforms[ "test" ] = true;
        m_intUniforms[ "test" ] = 10;
        m_floatUniforms[ "test" ] = 1.23f;
        m_gluintUniforms[ "test" ] = 99u;
        m_vec2Uniforms[ "test" ] = glm::vec2( 1.f, 10.f ); 
        m_vec3Uniforms[ "test" ] = glm::vec3( 1.f, 10.f, 1.24f );
        m_vec4Uniforms[ "test" ] = glm::vec4( 1.f, 10.f, 1.24f, 0.345f );
        m_mat3Uniforms[ "test" ] = glm::mat3( 1.24f );*/
    }

    bool ShaderProgram::attachShader( std::vector<Shader> const & shaders )
    {
        GLint success = GL_FALSE;

        if( m_shaders.empty() )
        {
            m_shaders = shaders;
        }
        else
        {
            Utils::Logger::LogMessage(Utils::LogType::Error, "Shader Warning! Attached shaders ignored as shader already has had shaders attached\n" );
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
                glGetProgramInfoLog( m_Id, infoLogSize, nullptr, &infoLogVec[ 0 ] );
                std::string infoLog( infoLogVec.begin(), infoLogVec.end() );
                std::string error = "Error::Shader::Program::Linking_Failed\n";
                error.append(infoLog);
                Utils::Logger::LogMessage(Utils::LogType::Error, error);
            }
            else
            {
                Utils::Logger::LogMessage(Utils::LogType::Error, "Error::Shader::Program::Linking_Failed\nNo error log output!");
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
        
        size_t const setBits = m_shaderTypeFlags.count();
        
        for( size_t i = 0; i < setBits; ++i )
        {
            if( m_shaderTypeFlags.test( i ) )
            {
                Shader::Type const type = static_cast<Shader::Type>(i);
                if( type < Shader::Type::Total && type != Shader::Type::Invalid )
                {
                    shaders.push_back( { type, m_shaderName.c_str() } );
                }
            }
        }

        return attachShader(shaders);
    }

    void ShaderProgram::useProgram() const noexcept
    {
        glUseProgram( m_Id );
    }

    void ShaderProgram::setUniform( const char * name, float x, float y, float z )
    {
        GLint const loc = getUniformLocation( name );
        glUniform3f( loc, x, y, z );
    }

    void ShaderProgram::setUniform( const char * name, glm::vec3 const & v )
    {
        this->setUniform( name, v.x, v.y, v.z );
        m_vec3Uniforms[ name ] = v;
    }

    void ShaderProgram::setUniform( const char * name, glm::vec4 const & v )
    {
        GLint const loc = getUniformLocation( name );
        glUniform4f( loc, v.x, v.y, v.z, v.w );
        m_vec3Uniforms[ name ] = v;
    }

    void ShaderProgram::setUniform( const char * name, glm::vec2 const & v )
    {
        GLint const loc = getUniformLocation( name );
        glUniform2f( loc, v.x, v.y );
        m_vec2Uniforms[ name ] = v;
    }

    void ShaderProgram::setUniform( const char * name, glm::mat4 const & m )
    {
        GLint const loc = getUniformLocation( name );
        glUniformMatrix4fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
        m_mat4Uniforms[ name ] = m;
    }

    void ShaderProgram::setUniform( const char * name, glm::mat3 const & m )
    {
        GLint const loc = getUniformLocation( name );
        glUniformMatrix3fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
        m_mat3Uniforms[ name ] = m;
    }

    void ShaderProgram::setUniform( const char * name, float val )
    {
        GLint const loc = getUniformLocation( name );
        glUniform1f( loc, val );
        m_floatUniforms[ name ] = val;
    }

    void ShaderProgram::setUniform( const char * name, int val )
    {
        GLint const loc = getUniformLocation( name );
        glUniform1i( loc, val );
        m_intUniforms[ name ] = val;
    }

    void ShaderProgram::setUniform( const char * name, GLuint val )
    {
        GLint const loc = getUniformLocation( name );
        glUniform1ui( loc, val );
        m_gluintUniforms[ name ] = val;
    }

    void ShaderProgram::setUniform( const char * name, bool val )
    {
        int const loc = getUniformLocation( name );
        glUniform1i( loc, val );
        m_boolUniforms[ name ] = val;
    }

    void ShaderProgram::printActiveUniforms() const
    {
        GLint numUniforms = 0;
        glGetProgramInterfaceiv( m_Id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms );

        GLenum const properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
        
        Utils::Logger::LogMessage(Utils::LogType::Info, "Active Uniforms\n");
        for( int i = 0; i < numUniforms; ++i )
        {
            GLint results[ 4 ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM, i, 4, properties, 4, NULL, results );

            if( results[ 3 ] != -1 )  // Skip uniforms in blocks
            {
                continue;
            }

            GLint const nameBufSize = results[ 0 ] + 1;
            char * name = new char[ nameBufSize ];
            glGetProgramResourceName( m_Id, GL_UNIFORM, i, nameBufSize, NULL, name );
            std::string uniform = fmt::format("{} {} ({})\n",results[2], name, getTypeString(results[1]) );
			Utils::Logger::LogMessage(Utils::LogType::Info, uniform);
            delete [] name;
        }
    }

    void ShaderProgram::printActiveUniformBlocks() const
    {
        GLint numBlocks = 0;
        glGetProgramInterfaceiv( m_Id, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks );
		GLenum const blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
		GLenum const blockIndex[] = { GL_ACTIVE_VARIABLES };
		GLenum const props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

        for( int block = 0; block < numBlocks; ++block )
        {
            GLint blockInfo[ 2 ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo );

            GLint const numUnis = blockInfo[ 0 ];
            char * blockName = new char[ blockInfo[ 1 ] + 1 ];
            glGetProgramResourceName( m_Id, GL_UNIFORM_BLOCK, block, blockInfo[ 1 ] + 1, NULL, blockName );
            
            std::string uniformBlockName = fmt::format( "Uniform Block \"{}\":\n", blockName);
            delete [] blockName;
            Utils::Logger::LogMessage(Utils::LogType::Info, uniformBlockName);

            GLint * unifIndexes = new GLint[ numUnis ];
            glGetProgramResourceiv( m_Id, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes );

            for( int unif = 0; unif < numUnis; ++unif )
            {
                GLint const uniIndex = unifIndexes[ unif ];
                GLint results[ 3 ];
                glGetProgramResourceiv( m_Id, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results );

                GLint const nameBufSize = results[ 0 ] + 1;
                char * name = new char[ nameBufSize ];
                glGetProgramResourceName( m_Id, GL_UNIFORM, uniIndex, nameBufSize, NULL, name );
				std::string uniformName = fmt::format("    {} ({})\n", name, getTypeString(results[1]));
                delete [] name;
				Utils::Logger::LogMessage(Utils::LogType::Info, uniformName);
            }

            delete [] unifIndexes;
        }
    }

    void ShaderProgram::printActiveAttribs() const
    {
        GLint numAttribs;
        glGetProgramInterfaceiv( m_Id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs );

        GLenum const properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

        printf( "Active attributes:\n" );
		Utils::Logger::LogMessage(Utils::LogType::Info, "Active attributes:\n");
        for( int i = 0; i < numAttribs; ++i )
        {
            GLint results[ 3 ];
            glGetProgramResourceiv( m_Id, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results );

            GLint const nameBufSize = results[ 0 ] + 1;
            char * name = new char[ nameBufSize ];
            glGetProgramResourceName( m_Id, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name );
			std::string attribute = fmt::format("{} {} ({})\n", results[2], name, getTypeString(results[1]));
            delete [] name;
			Utils::Logger::LogMessage(Utils::LogType::Info, attribute);
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
				std::string uniformBlockName = fmt::format("Unable to find Uniform Location: {}. Check the {} shader!\n", name, m_shaderName );
				Utils::Logger::LogMessage(Utils::LogType::Info, uniformBlockName);
            }
        }

        return uniformLocations[ name ];
    }

    const char * ShaderProgram::getTypeString( GLenum type ) const noexcept
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

	std::string const & ShaderProgram::GetShaderName() const
	{
        return m_shaderName;
	}

	std::string const & ShaderProgram::GetShaderPath( Shader::Type type ) const
	{
		static std::string emptySource = "";
		for ( auto const & shader : m_shaders )
		{
			if ( shader.getType() == type )
			{
				return shader.getPath();
			}
		}
		return emptySource;
	}

	std::string const & ShaderProgram::GetShaderSource( Shader::Type type ) const
	{
        static std::string emptySource = "";
        for ( auto const & shader : m_shaders )
        {
            if ( shader.getType() == type )
            {
                return shader.getSource();
            }
        }
        return emptySource;
	}

	Utils::EnumBitmask<Graphics::Shader::Type> const & ShaderProgram::GetShaderTypes() const
	{
        return m_shaderTypeFlags;
	}

    std::map<std::string, int> const & ShaderProgram::GetUniforms() const
	{
        return uniformLocations;
	}

	std::map<std::string, bool> const & ShaderProgram::GetBoolUniforms() const
	{
        return m_boolUniforms;
	}

	std::map<std::string, int> const & ShaderProgram::GetIntUniforms() const
	{
        return m_intUniforms;
	}

	std::map<std::string, float> const & ShaderProgram::GetFloatUniforms() const
	{
        return m_floatUniforms;
	}

	std::map<std::string, GLuint> const & ShaderProgram::GetGLuintUniforms() const
	{
        return m_gluintUniforms;
	}

	std::map<std::string, glm::vec2> const & ShaderProgram::GetVec2Uniforms() const
	{
        return m_vec2Uniforms;
	}

	std::map<std::string, glm::vec3> const & ShaderProgram::GetVec3Uniforms() const
	{
        return m_vec3Uniforms;
	}

	std::map<std::string, glm::vec4> const & ShaderProgram::GetVec4Uniforms() const
	{
        return m_vec4Uniforms;
	}

	std::map<std::string, glm::mat3> const & ShaderProgram::GetMat3Uniforms() const
	{
        return m_mat3Uniforms;
	}

	std::map<std::string, glm::mat4> const & ShaderProgram::GetMat4Uniforms() const
	{
        return m_mat4Uniforms;
	}
}
