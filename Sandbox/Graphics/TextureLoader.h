//#pragma once
//
//#include <string>
//#include <map>
//
//#include "Texture.h"
//
//namespace Graphics
//{
//
//    class TextureLoader
//    {
//    public:
//        using TextureMap = std::map<std::string, Texture>;
//
//        TextureLoader();
//        ~TextureLoader();
//
//        TexturePtr GetTexture( const char * texturePath );
//
//        TextureMap const & GetLoadedTextures();
//    private:
//        bool LoadTexture( const std::string & texturePath, TexturePtr & outTexture );
//        bool FindTexture( const std::string & texturePath, TexturePtr & outTexture );
//
//        bool m_showTextureWindow { false };
//
//        TextureMap m_loadedTextures;
//
//        Texture * m_defaultTexture;
//    };
//}
