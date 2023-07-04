#version 450

layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColour;
layout (location=2) in float aThickness;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
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
	gl_Position = u_ViewProjection * model* vec4(aPos, 1.0);
}