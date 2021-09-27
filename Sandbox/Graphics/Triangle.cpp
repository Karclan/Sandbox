#include "Triangle.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    Triangle::Triangle()
    {
        create();
    }

    Triangle::~Triangle()
    {
    }

    void Triangle::create()
    {
        if (!getCreated())
        {
            static constexpr float vertices [] = {
                /*Vertex*/ -0.5f, -0.5f, 0.0f, /*Colour*/ 1.0f, 0.f, 0.f, /*Text Coord*/ 1.0f, 1.0f, // left  
                /*Vertex*/  0.5f, -0.5f, 0.0f, /*Colour*/ 0.0f, 1.f, 0.f, /*Text Coord*/ 1.0f, 0.0f, // right 
                /*Vertex*/  0.0f,  0.5f, 0.0f, /*Colour*/ 0.0f, 0.f, 1.f, /*Text Coord*/ 0.0f, 0.0f, // top   
            };

            static constexpr unsigned int indices [] = {
                0, 1, 2
            };

            bindVAO();
            bindVertices( vertices, sizeof( vertices ), 8 );
            bindIndices( indices, sizeof( indices ) );
            finishBinding();
            setCreated( true );
        }
    }

    //void Triangle::draw()
    //{
    //    glBindVertexArray(m_vao);
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //}
}
