#pragma once

#include <cstdint>
#include "glad/glad.h"

namespace Graphics
{
    using TextureId = unsigned int;

    // Texture Class
    // 
    //
    class Texture
    {
    public:
        struct InfoStruct
        {
            enum class Format
            {
                JPG,
                PNG,
                UNKNOWN,
            };

            int m_height { 0 };
            int m_width { 0 };
            int m_numChannels { 0 };
            TextureId m_textureId { 0 };

            Format m_format { Format::UNKNOWN };
        };

        Texture( InfoStruct info );

        ~Texture();

        TextureId GetTextureID() const;

        int GetHeight() const;
        int GetWidth() const;
        int GetNumChannels() const;

    private:
        InfoStruct m_info;

    };

    // Using declaration for const pointer to texture
    using TexturePtr = Texture const *;
    using TextureRef = Texture const &;
}
