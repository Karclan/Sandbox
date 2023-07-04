#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
};

uniform mat4 model;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = u_ViewProjection * model* vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}