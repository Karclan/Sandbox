#include "Cube.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    Cube::Cube()
    {
        create();
    }

    Cube::~Cube()
    {
    }

    void Cube::create()
    {
        if( !getCreated() )
        {
            static constexpr float vertices [] = {
                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/  0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/  0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,

                /*Vertex*/  0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/ -0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,



                /*Vertex*/ -0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/  0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,

                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/ -0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/ -0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,



                /*Vertex*/ -0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/ -0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,

                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/ -0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/ -0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,



                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/  0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/  0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,

                /*Vertex*/  0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/  0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,



                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/  0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/  0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,

                /*Vertex*/  0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/ -0.5f, -0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/ -0.5f, -0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,



                /*Vertex*/ -0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f,
                /*Vertex*/  0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 1.0f,
                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,

                /*Vertex*/  0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 1.0f, 0.0f,
                /*Vertex*/ -0.5f,  0.5f,  0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 0.0f,
                /*Vertex*/ -0.5f,  0.5f, -0.5f, /*Colour*/ 1.0f, 1.0f, 1.0f, /*Text Coord*/ 0.0f, 1.0f
            };

            bindVAO();
            bindVertices( vertices, sizeof( vertices ), 8 );
            finishBinding();
            setCreated( true );
        }
    }

    void Cube::draw() const
    {
        glBindVertexArray( m_vao );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
    }
}

/*
*/