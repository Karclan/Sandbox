#pragma once

#include "../Core/ResourceManager.h"

#include "ShaderProgram.h"

#include <unordered_map>

namespace Graphics
{
	class TShaderResourceHandler : public Core::TResourceTypeHandler<Core::ResourceType::Shader, SB::Shared<ShaderProgram>>
	{
	public:
		struct ShaderResource
		{
			ShaderId Id;
			std::string Name;
			SB::Shared<ShaderProgram> Resource;

			bool operator==( ShaderId const & id ) const
			{
				return Id == id;
			}

			bool operator==( std::string const & name ) const
			{
				return Name == name;
			}

			//Special case where a shader key can be the same as other keys if the ID or Name matches.
			//As we have situations we want to find a shader by name and then also by ID;
			bool operator==( ShaderResource const & otherKey ) const
			{
				bool bSame = true;
				bSame &= Id == otherKey.Id;
				bSame &= Name == otherKey.Name;
				return bSame;
			}
		};
		using ShaderContainer = std::vector<ShaderResource>;

		SB::Shared<ShaderProgram> GetOrLoadResource( char const * resourceName ) override;
		SB::Shared<ShaderProgram> GetShader( ShaderComponent const & shaderComponent );
		ShaderContainer const& GetShaders() const { return m_loadedShaders; }

		void BuildWindow() override;

	private:
		using ShaderPathsList = std::unordered_map<Shader::Type, std::string>;
		ShaderPathsList GenerateShaderPaths( std::string const & shaderName );
		std::string LoadCodeFromFile( char const * filePath );
		Shader LoadShaderCode( std::pair<Shader::Type, std::string > const & pair );
		Shader LoadShaderCode( Shader::Type type, std::string const & shaderPath );

		SB::Shared<ShaderProgram> LoadShader( char const * resourceName );
		
		ShaderContainer m_loadedShaders;
	};
}
