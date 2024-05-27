#version 450

layout (location=0) in vec2 aPos;
layout (location=1) in vec4 aColour;
layout (location=2) in float aThickness;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

uniform mat4 model;

out VS_Out
{
    vec4 VertColour;
    float Thickness;
} vs_out;

void main()
{
	vs_out.VertColour = aColour;
	vs_out.Thickness = aThickness;
	gl_Position = u_View * model* vec4(aPos, 0.0, 1.0);
}