#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "../Utils/Logger.h"

namespace Graphics
{
	Shader::Shader()
	{}

	Shader::Shader( Type type, char const * filePath )
	{
		/*std::string path = "../Sandbox/Resources/Shaders/";
		path.append( filePath );*/
		create( type, filePath );
	}

	Shader::~Shader()
	{}

	bool Shader::create( Type type, const char * filePath )
	{
		int compileSuccess = GL_FALSE;

		m_type = type;
		load( filePath );

		if ( !m_shaderSource.empty() )
		{
			switch ( m_type )
			{
				case Type::Vertex:
					m_Id = glCreateShader( GL_VERTEX_SHADER );
					break;
				case Type::Fragment:
					m_Id = glCreateShader( GL_FRAGMENT_SHADER );
					break;
				case Type::Geometry:
					m_Id = glCreateShader( GL_GEOMETRY_SHADER );
				case Type::Invalid:
				default:
					break;
			}

			char const * c_str = m_shaderSource.c_str();
			glShaderSource( m_Id, 1, &c_str, NULL );
			glCompileShader( m_Id );

			glGetShaderiv( m_Id, GL_COMPILE_STATUS, &compileSuccess );
			if ( compileSuccess == GL_FALSE )
			{
				int infoLogSize;
				glGetShaderiv( m_Id, GL_INFO_LOG_LENGTH, &infoLogSize );
				std::vector<GLchar> infoLogVec( infoLogSize );
				glGetShaderInfoLog( m_Id, 512, NULL, &infoLogVec[0] );
				std::string infoLog( infoLogVec.begin(), infoLogVec.end() );
				std::string error = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
				error.append( infoLog );
				Utils::Logger::LogMessage( Utils::LogType::Error, error );
			}
		}

		return compileSuccess == GL_TRUE;
	}

	void Shader::cleanUp()
	{
		m_shaderSource.clear();
		m_Id = 0;
		m_type = Type::Invalid;
	}

	void Shader::load( char const * filePath )
	{
		m_shaderFilePath = filePath;
		m_shaderSource.clear();
		std::ifstream shaderFile;
		shaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );// ensure ifstream objects can throw exceptions:
		try
		{
			shaderFile.open( m_shaderFilePath );// open files
			std::stringstream vShaderStream;
			vShaderStream << shaderFile.rdbuf();// read file's buffer contents into streams
			shaderFile.close();// close file handlers
			m_shaderSource = vShaderStream.str();// convert stream into string
		}
		catch ( std::ifstream::failure e )
		{
			std::string error = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\nFile: ";
			error.append( filePath );
			Utils::Logger::LogMessage( Utils::LogType::Error, error );
		}
	}
}
