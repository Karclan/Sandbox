#version 450

struct VS_Out
{
    vec3 VertColour;
    vec2 TexCoord;
    float TextureIndex;
    float TextureCount;
    float TilingFactor;
	float TextureId;
}; 

layout (location=0) in VS_Out fs_in;

// texture samplers

layout (binding = 0) uniform sampler2D textures[32];

layout (location=0) out vec4 FragColor;
layout (location=1) out int FragColor2;

void main()
{
	if(int(VS_Out.TextureId) > -1 && int(VS_Out.TextureId) < 32)
	{
		vec4 textureSample = texture(textures[int(fs_in.TextureId)], fs_in.TexCoord * fs_in.TilingFactor);	
		//Not sure if this is the best way of doing it?
		if(textureSample.a < 0.1)
		{
			discard;
		}
	}

	FragColor = textureSample;
}