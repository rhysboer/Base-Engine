#version 420 core

in vec2 _texCoords;
in vec3 _normals;

out vec4 FragColor;

void main()
{
	FragColor = vec4(_texCoords.x, _texCoords.y, 1, 1); 
}