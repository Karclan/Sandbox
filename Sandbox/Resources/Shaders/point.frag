#version 450

in VS_Out
{
    vec4 Colour;
    float Size;
} fr_In;

layout(location=0) out vec4 FragColour;

void main()
{
    FragColour = fr_In.Colour;
}