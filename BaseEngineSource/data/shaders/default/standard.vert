#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 uvs; // Tex Coordinates

layout (std140) uniform Camera
{
	mat4 projectionView;
};

uniform mat4 model;

out vec3 _normals;
out vec2 _uvs;

void main()
{
	_normals = normals;
	_uvs = uvs;
	
	gl_Position = projectionView * model * vec4(vertex, 1);
}