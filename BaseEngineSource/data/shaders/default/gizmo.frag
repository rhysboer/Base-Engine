#version 330 core

in vec3 _colour;

out vec4 FragColor;

void main()
{
	FragColor = vec4(_colour, 1.0);
}