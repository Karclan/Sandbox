#version 450

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColour;
layout (location=2) in float aRadius;
layout (location=3) in int aPointCount;

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
    float Radius;
    int PointCount;
} VS_out;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
    VS_out.Radius = aRadius; 
    VS_out.Colour = aColour;
    VS_out.PointCount = aPointCount;
}