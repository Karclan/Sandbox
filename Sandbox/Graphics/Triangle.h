#pragma once

#include "Mesh.h"

namespace Graphics
{
    class Triangle : public Mesh
    {
    public:
        Triangle();
        virtual ~Triangle() = default;

        // Inherited via Mesh
        virtual void Create() override;

        // Inherited via Mesh
        //virtual void draw() override;
    };
}
