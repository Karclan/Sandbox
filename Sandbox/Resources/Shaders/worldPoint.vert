#version 450

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColour;
layout (location=2) in float aSize;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

uniform mat4 model;

out VS_Out
{
    vec4 Colour;
    float Size;
} vs_out;

void main()
{
    vs_out.Size = aSize; 
    vs_out.Colour = aColour;
    gl_Position = u_ViewProjection * model * vec4(aPos, 1.0);
}