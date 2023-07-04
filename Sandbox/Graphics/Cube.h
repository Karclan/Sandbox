#pragma once

#include "Mesh.h"

namespace Graphics
{
    class Cube : public Mesh
    {
    public:
        Cube();
        virtual ~Cube() = default;
        // Inherited via Mesh
        virtual void Create() override;
    };
}
