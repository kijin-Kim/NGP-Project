#version 330 core

out vec4 outColor;
in vec2 f_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	outColor = texture(u_Texture, f_TexCoord);
}