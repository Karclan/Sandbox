#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "../Imgui/imgui.h"
#include "../Utils/Logger.h"

namespace Graphics
{
    TextureLoader::TextureLoader()
        : m_loadedTextures()
        , m_defaultTexture( nullptr )
    {
        stbi_set_flip_vertically_on_load( true ); // tell stb_image.h to flip loaded texture's on the y-axis.
    }

    TextureLoader::~TextureLoader()
    {

    }

    TexturePtr TextureLoader::getTexture( const char * texturePath )
    {
        std::string path = "../Sandbox/Resources/Textures/";
        path.append( texturePath );
        TexturePtr returnTexture = m_defaultTexture;
        if( !findTexture( path, returnTexture ) )
        {
            loadTexture( path, returnTexture );
        }
        return returnTexture;
    }

    TextureLoader::TextureMap const & TextureLoader::getLoadedTextures()
    {
        return m_loadedTextures;
    }

    bool TextureLoader::loadTexture( const std::string & texturePath, TexturePtr & outTexture )
    {
        Texture::InfoStruct textureInfo;

        static const std::string jpgExt = ".jpg";
        static const std::string pngExt = ".png";

        if( 0 == texturePath.compare( texturePath.length() - jpgExt.length(), jpgExt.length(), jpgExt ) )
        {
            textureInfo.m_format = Texture::InfoStruct::Format::JPG;
        }
        else if( 0 == texturePath.compare( texturePath.length() - pngExt.length(), pngExt.length(), pngExt ) )
        {
            textureInfo.m_format = Texture::InfoStruct::Format::PNG;
        }

        // ---------
        glGenTextures( 1, &textureInfo.m_textureId );
        glBindTexture( GL_TEXTURE_2D, textureInfo.m_textureId );
        // set the texture wrapping parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // set texture filtering parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // load image, create texture and generate mipmaps
        stbi_uc * data = stbi_load( texturePath.c_str(), &textureInfo.m_width, &textureInfo.m_height, &textureInfo.m_numChannels, 0 );
        if( data )
        {
            switch( textureInfo.m_format )
            {
            case Texture::InfoStruct::Format::JPG:
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureInfo.m_width, textureInfo.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
                glGenerateMipmap( GL_TEXTURE_2D );
                break;

            case Texture::InfoStruct::Format::PNG:
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureInfo.m_width, textureInfo.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
                glGenerateMipmap( GL_TEXTURE_2D );
                break;
            case Texture::InfoStruct::Format::UNKNOWN:
            default:
                Utils::Logger::LogMessage(Utils::LogType::Error, "Texture loader, unknown format!\n");
                return false;
            }

            outTexture = &m_loadedTextures.emplace( texturePath, Texture( textureInfo ) ).first->second;
        }
        else
        {
            Utils::Logger::LogMessage(Utils::LogType::Error, "Failed to load texture.\n");
            return false;
        }
        stbi_image_free( data );

        return true;
    }

    bool TextureLoader::findTexture( const std::string & texturePath, TexturePtr & outTexture )
    {
        TextureMap::iterator findResult = m_loadedTextures.find( texturePath );
        if( findResult != m_loadedTextures.end() )
        {
            outTexture = &findResult->second;
            return true;
        }

        return false;
    }

}
