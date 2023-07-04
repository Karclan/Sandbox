#pragma once
#include "Mesh.h"

namespace Graphics
{
    class Square : public Mesh
    {
    public:
        Square();
        virtual ~Square() = default;
        // Inherited via Mesh
        void Create() override;
    };
}
