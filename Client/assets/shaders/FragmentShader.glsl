#version 330 core

out vec4 outColor;
in vec2 f_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform int u_bToggleTexture;
uniform int u_bToggleColor;

void main()
{
    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if(u_bToggleTexture == 1)
		outColor = texture(u_Texture, f_TexCoord);
	if(u_bToggleColor == 1)
		outColor *= u_Color;
}