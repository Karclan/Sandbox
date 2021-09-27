#include "Texture.h"

namespace Graphics
{
    Texture::Texture( InfoStruct info )
        : m_info( info )
    {

    }

    Texture::~Texture()
    {
    }

    GLuint Texture::GetTextureID() const
    {
        return m_info.m_textureId;
    }

    int Texture::GetHeight() const
    {
        return m_info.m_height;
    }

    int Texture::GetWidth() const
    {
        return m_info.m_width;
    }

    int Texture::GetNumChannels() const
    {
        return m_info.m_numChannels;
    }

}
