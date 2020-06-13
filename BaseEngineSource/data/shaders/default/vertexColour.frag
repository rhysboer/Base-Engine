#version 330 core

in vec2 _texCoords;
in vec3 _normals;
in vec3 _vertexColour;

out vec4 FragColor;

void main()
{
	FragColor = vec4(_vertexColour, 1.0);
}