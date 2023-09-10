#pragma once

#include "Texture.h"
#include <vector>

namespace Graphics
{
    struct TextureComponent
    {
        std::vector<TextureId> TextureList;
    };
}