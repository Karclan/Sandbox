#version 450

layout (location = 0) out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 tex1Sample = texture(texture1, TexCoord);
	vec4 tex2Sample = texture(texture2, TexCoord);
	vec4 texCoordColours = vec4(TexCoord.x, TexCoord.y, 0.f, 1.f);// texture(texture2, TexCoord);
	
    if(tex1Sample.a < 0.1)
	{
	 	discard;
	}
	FragColor = texCoordColours;
}