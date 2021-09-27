#pragma once

#include "Mesh.h"

namespace Graphics
{
    class Triangle : public Mesh
    {
    public:
        Triangle();
        virtual ~Triangle();

        // Inherited via Mesh
        virtual void create() override;

        // Inherited via Mesh
        //virtual void draw() override;
    };
}
