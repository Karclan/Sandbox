#include "ShaderResourceHandler.h"

#include "ShaderComponent.h"

#include "../Utils/TemplateUtils.h"

#include <algorithm>

namespace Graphics
{
	

	SB::Shared<ShaderProgram> TShaderResourceHandler::GetOrLoadResource( char const * resourceName )
	{
		ShaderContainer::const_iterator it = std::find_if( m_loadedShaders.begin(), m_loadedShaders.end(), [&resourceName] ( ShaderResource & shaderResource ) { return shaderResource == resourceName; } );
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
		ShaderContainer::const_iterator it = std::find_if( m_loadedShaders.begin(), m_loadedShaders.end(), [ &Id = shaderComponent.ShaderId ]( ShaderResource & shaderResource ) { return shaderResource == Id; } );
		SB_CORE_ASSERT( it != m_loadedShaders.end(), fmt::format( "Unabled to locate shader: {}", shaderComponent.ShaderId ) );
		return it->Resource;
	}

	void TShaderResourceHandler::BuildWindow()
	{

	}

	std::string TShaderResourceHandler::GenerateShaderPaths( std::string const & shaderName )
	{
		std::string path;
		constexpr char const * c_ShaderPath = "../Sandbox/Resources/Shaders/";
		path.append( c_ShaderPath );
		create( type, path.c_str() );
	}

	std::string TShaderResourceHandler::LoadShaderCode( std::string const & shaderPath )
	{

	}

	SB::Shared<ShaderProgram> TShaderResourceHandler::LoadShader( char const * resourceName )
	{
		SB::Shared<ShaderProgram> returnProgram; 


		return returnProgram;
	}
}
