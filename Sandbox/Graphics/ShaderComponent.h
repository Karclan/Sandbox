#pragma once

#include "ShaderProgram.h"

#include <memory>

namespace Graphics
{
    struct ShaderComponent
    {
        unsigned int ShaderId { 0 };
        std::weak_ptr<IShaderHandle> ShaderHandle { };
    };
}
