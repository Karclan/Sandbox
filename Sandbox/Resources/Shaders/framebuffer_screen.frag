#version 450

layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec4 extraColour = vec4(0.5, 0.0, 0.0, 1.0);
	vec3 col = texture(screenTexture, TexCoord).rgb;
    FragColor = mix(vec4(col, 1.0), extraColour, 0.5);
}