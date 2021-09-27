#pragma once

#include "Mesh.h"

namespace Graphics
{
    class Cube : public Mesh
    {
    public:
        Cube();
        virtual ~Cube();

        // Inherited via Mesh
        virtual void create() override;

        // Inherited via Mesh
        virtual void draw() const override;
    };
}
