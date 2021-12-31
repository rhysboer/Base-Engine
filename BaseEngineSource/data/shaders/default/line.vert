#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 colour; // Colour

uniform mat4 model;
uniform mat4 projectionView;

out vec3 _colour;

void main()
{
	_colour = colour;
	
	gl_Position = projectionView * model * vec4(vertex, 1);
}