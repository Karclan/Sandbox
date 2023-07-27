#pragma once

#include "../Core/ResourceManager.h"

#include "ShaderProgram.h"

#include <vector>

namespace Graphics
{
	class TShaderResourceHandler : public Core::TResourceTypeHandler<Core::ResourceType::Shader, ShaderProgram>
	{
	public:
		SB::Shared<ShaderProgram> GetOrLoadResource( char const * resourceName ) override;
		SB::Shared<ShaderProgram> GetShader( ShaderComponent const & shaderComponent );

		void BuildWindow() override;
	private:

		std::string GenerateShaderPaths( std::string const & shaderName );
		std::string LoadShaderCode( std::string const & shaderPath );

		SB::Shared<ShaderProgram> LoadShader( char const * resourceName );

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
		ShaderContainer m_loadedShaders;
	};
}
