#version 450

out vec4 FragColor;

in VS_Out
{
    vec4 VertColour;
    float Thickness;
} fs_in;

void main()
{
	FragColor = fs_in.VertColour;
}