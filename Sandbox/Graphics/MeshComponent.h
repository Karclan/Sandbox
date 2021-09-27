#pragma once

namespace Graphics
{
    enum class PrimativeType
    {
        Square,
        Triangle,
        Cube,
        Invalid,
    };

    struct MeshComponent
    {
        PrimativeType type { PrimativeType::Invalid };
        unsigned int MeshId { 0 };
    };
}