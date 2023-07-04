#version 450

out vec4 FragColor;  
in vec3 vertexColor;

uniform vec4 colourMod;

void main()
{
    FragColor = vec4(vertexColor, 1.0)* colourMod;
    //* colourMod
}