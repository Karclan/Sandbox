#version 450

in VS_Out
{
    vec4 Colour;
    float Size;
} fs_in;

layout(location=0) out vec4 FragColour;

void main()
{
    FragColour = fs_in.Colour;
}