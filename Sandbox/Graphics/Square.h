#pragma once
#include "Mesh.h"

namespace Graphics
{
    class Square : public Mesh
    {
    public:
        Square();
        virtual ~Square();
        // Inherited via Mesh
        virtual void create() override;
        // Inherited via Mesh
        //virtual void draw() override;
    };
}
