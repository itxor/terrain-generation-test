#version 440 core

layout (location = 0) in vec3 position;
//out vec4 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}