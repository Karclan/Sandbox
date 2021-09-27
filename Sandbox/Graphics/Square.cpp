#include "Square.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    Square::Square()
    {
        create();
    }

    Square::~Square()
    {
    }

    void Square::create()
    {
        if (!getCreated())
        {
            static constexpr float vertices [] = {
                /*Vertex*/  0.5f,  0.5f, 0.0f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 1.0f, 1.0f, // top right
                /*Vertex*/  0.5f, -0.5f, 0.0f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 1.0f, 0.0f, // bottom right
                /*Vertex*/ -0.5f, -0.5f, 0.0f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 0.0f, 0.0f, // bottom left
                /*Vertex*/ -0.5f,  0.5f, 0.0f, /*Colour*/ 1.f,  1.f, 1.f, /*Text Coord*/ 0.0f, 1.0f, // top left 
            };

            static constexpr unsigned int indices [] = {
                 0, 1, 3,   // first triangle
                 1, 2, 3    // second triangle
            };

            bindVAO();
            bindVertices( vertices, sizeof( vertices ), 8 );
            bindIndices( indices, sizeof( indices ) );
            finishBinding();
            setCreated( true );
        }
    }

    //void Square::draw()
    //{
    //    glBindVertexArray(m_vao);
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //}
}
