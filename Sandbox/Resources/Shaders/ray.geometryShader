#version 330 core

layout (location=0) in vec3 aPos;

layout (points ) in; 
layout (points, max_vertices = 1) out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
};