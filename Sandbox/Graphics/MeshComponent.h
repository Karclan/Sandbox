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

    using MeshIdType = unsigned int;

    struct MeshComponent
    {
        PrimativeType Type { PrimativeType::Invalid };
        MeshIdType MeshId { 0 };
    };
}