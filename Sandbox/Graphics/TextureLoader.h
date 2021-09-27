#pragma once

#include <string>
#include <map>

#include "Texture.h"

namespace Graphics
{

    class TextureLoader
    {
    public:
        using TextureMap = std::map<std::string, Texture>;

        TextureLoader();
        ~TextureLoader();

        TexturePtr getTexture( const char * texturePath );

        TextureMap const & getLoadedTextures();
    private:
        bool loadTexture( const std::string & texturePath, TexturePtr & outTexture );
        bool findTexture( const std::string & texturePath, TexturePtr & outTexture );

        bool m_showTextureWindow { false };

        TextureMap m_loadedTextures;

        Texture * m_defaultTexture;
    };
}
