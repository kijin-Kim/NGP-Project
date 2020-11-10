#version 330 core
layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Proj;

out vec2 f_TexCoord;

void main()
{
	f_TexCoord = v_TexCoord;
	gl_Position = u_Proj * u_Model * vec4(v_Position, 1.0f);
}