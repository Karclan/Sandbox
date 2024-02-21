#include "ShaderResourceHandler.h"

#include "ShaderComponent.h"

#include "../Utils/TemplateUtils.h"
#include "../magic_enum/magic_enum.hpp"

#include <algorithm>
#include <array>

#include <sstream>
#include <fstream>
#include <filesystem>

namespace Graphics
{
	SB::Shared<ShaderProgram> TShaderResourceHandler::GetOrLoadResource( char const * resourceName )
	{
		ShaderContainer::const_iterator it = std::find_if( m_loadedShaders.begin(), m_loadedShaders.end(), [ &resourceName ] ( ShaderResource & shaderResource )
														   {
															   return shaderResource == resourceName;
														   } );
		if ( it != m_loadedShaders.end() )
		{
			return it->Resource;
		}
		else
		{
			return LoadShader( resourceName );
		}
	}

	SB::Shared<ShaderProgram> TShaderResourceHandler::GetShader( ShaderComponent const & shaderComponent )
	{
		ShaderContainer::const_iterator it = std::find_if( m_loadedShaders.begin(), m_loadedShaders.end(), [ &Id = shaderComponent.ShaderId ] ( ShaderResource & shaderResource )
														   {
															   return shaderResource == Id;
														   } );
		SB_CORE_ASSERT( it != m_loadedShaders.end(), fmt::format( "Unabled to locate shader: {}", shaderComponent.ShaderId ) );
		return it->Resource;
	}

	void TShaderResourceHandler::BuildWindow()
	{

	}

	SB::Shared<ShaderProgram> TShaderResourceHandler::LoadShader( char const * resourceName )
	{
		SB::Shared<ShaderProgram> returnProgram;

		ShaderPathsList shaderPaths = GenerateShaderPaths( resourceName );

		std::vector<Shader> shaders;
		for ( auto const & path : shaderPaths )
		{
			shaders.push_back( LoadShaderCode( path ) );
		}

		returnProgram = std::make_shared<ShaderProgram>( resourceName );

		bool bCreated = returnProgram->create( shaders );

		SB_CORE_ASSERT( bCreated, fmt::format( "Failed to create shader: {}", resourceName ) )
		//{
			ShaderResource resource;
			resource.Id = returnProgram->GetShaderId();
			resource.Name = resourceName;
			resource.Resource = returnProgram;

			m_loadedShaders.push_back( resource );
		//}
		//else
		//{
		//	SB_CORE_ASSERT( false, fmt::format( "Failed to load shader {}, check logs", resourceName ) );
		//}
		return returnProgram;
	}

	TShaderResourceHandler::ShaderPathsList TShaderResourceHandler::GenerateShaderPaths( std::string const & shaderName )
	{
		ShaderPathsList paths;
		std::string mainPath;
		constexpr char const * c_ShaderPath = "../Sandbox/Resources/Shaders/";
		mainPath.append( c_ShaderPath );
		mainPath.append( shaderName );

		auto const & enumValues = magic_enum::enum_values<Shader::Type>();

		for ( Shader::Type const & enumVal : enumValues )
		{
			std::string shaderFileExtension = "";
			std::string fullshaderName = mainPath;
			//Get shader file extension from type;
			switch ( enumVal )
			{
				case Shader::Type::Fragment:
				{
					shaderFileExtension = ".frag";
					break;
				}
				case Shader::Type::Vertex:
				{
					shaderFileExtension = ".vert";
					break;
				}
				case Shader::Type::Geometry:
				{
					shaderFileExtension = ".geom";
					break;
				}
				default:
				{
					SB_CORE_ASSERT( false, fmt::format( "Unhandled shader type!" ) );
				}
				case Shader::Type::Invalid:
				case Shader::Type::Total:
					break;
			}

			if ( !shaderFileExtension.empty() )
			{
				fullshaderName.append( shaderFileExtension );
				if ( std::filesystem::exists( ( fullshaderName ) ) )
				{
					paths.insert( { enumVal, fullshaderName } );
				}
			}
		}

		return paths;
	}

	std::string TShaderResourceHandler::LoadCodeFromFile( char const * filePath )
	{
		std::string shaderSource;

		std::ifstream shaderFile;
		shaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );// ensure ifstream objects can throw exceptions:
		try
		{
			shaderFile.open( filePath );// open files
			std::stringstream vShaderStream;
			vShaderStream << shaderFile.rdbuf();// read file's buffer contents into streams
			shaderFile.close();// close file handlers
			shaderSource = vShaderStream.str();// convert stream into string
		}
		catch ( std::ifstream::failure e )
		{
			std::string error = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\nFile: ";
			error.append( filePath );
			Utils::Logger::LogMessage( Utils::LogType::Error, error );
		}

		return shaderSource;
	}

	Shader TShaderResourceHandler::LoadShaderCode( std::pair<Shader::Type, std::string> const & pair )
	{
		return LoadShaderCode( pair.first, pair.second );
	}

	Shader TShaderResourceHandler::LoadShaderCode( Shader::Type type, std::string const & shaderPath )
	{
		
		return Shader( { type, shaderPath.c_str() } );
	}

	
}
