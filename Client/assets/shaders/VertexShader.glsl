#version 330 core
layout(location = 0) in vec4 position;

uniform mat4 u_Model;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_Model * position;
}