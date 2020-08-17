// Red Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

void main()
{
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
}

#type fragment
#version 330 core

out vec3 color;

void main()
{
	color = vec3(0,0,0);
}