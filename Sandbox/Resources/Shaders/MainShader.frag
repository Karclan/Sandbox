#version 450

in VS_Out
{
    vec3 VertColour;
    vec2 TexCoord;
    float TextureIndex;
    float TextureCount;
    float TilingFactor;
} fs_in;


// texture samplers
uniform sampler2D texture1;

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 FragColor2;

void main()
{
	vec4 tex1Sample = texture(texture1, fs_in.TexCoord);
	
	if(tex1Sample.a < 0.1)
	{
		discard;
	}
	else
	{
		FragColor = tex1Sample;
	}
}