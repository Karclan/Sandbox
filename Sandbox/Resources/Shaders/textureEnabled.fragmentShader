#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	vec4 tex1Sample = texture(texture1, TexCoord);
	
	if(tex1Sample.a < 0.1)
	{
		discard;
	}
	else
	{
		FragColor = tex1Sample;
	}
}