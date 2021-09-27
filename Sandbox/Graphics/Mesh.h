#pragma once

#include "MeshComponent.h"

namespace Graphics
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        virtual void create() = 0;
        virtual void draw() const;
        void bind();
        void cleanUp();

protected:
    void bindVAO();//Will generate if not made
    void bindVertices( float const * const vertices, unsigned int size, unsigned int stride );
    void bindIndices( unsigned int const * const indices, unsigned int size );
    void finishBinding();

    void setCreated( bool flag );
    bool getCreated() const;

    unsigned int m_vbo { 0 };
    unsigned int m_vao { 0 };
    unsigned int m_ebo { 0 };
protected:

    bool m_created { false };
    bool m_hasIndicies { false };
};
}
