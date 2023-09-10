#pragma once

#include "../Core/ResourceManager.h"

#include "Texture.h"
#include "TextureComponent.h"
#include "TextureLoader.h"

namespace Graphics
{
	class TTextureResourceHandler : public Core::TResourceTypeHandler<Core::ResourceType::Texture, SB::Shared<Texture>>
	{
	public:
		TTextureResourceHandler();
		~TTextureResourceHandler() = default; 

		SB::Shared<Texture> GetOrLoadResource( char const * resourceName ) override;
		SB::Shared<Texture> GetTexture( const char * texturePath );
		void BuildWindow() override;	


		using TextureMap = std::map<std::string, SB::Shared<Texture>>;
		TextureMap const & GetLoadedTextures();

	private:
		bool LoadTexture( const std::string & texturePath, SB::Shared<Texture> & outTexture );
		bool FindTexture( const std::string & texturePath, SB::Shared<Texture> outTexture );

		bool m_showTextureWindow { false };

		TextureMap m_loadedTextures;

		SB::Shared<Texture> m_defaultTexture;
	};
}
