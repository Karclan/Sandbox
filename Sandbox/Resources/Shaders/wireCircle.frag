#version 450

in vec4 circleColour; 

layout(location=0) out vec4 FragColour;

void main()
{
    FragColour = circleColour;
}