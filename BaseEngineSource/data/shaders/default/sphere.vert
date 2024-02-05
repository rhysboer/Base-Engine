#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 textureCoords; // Tex Coordinates

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out vec2 _texCoords;
out vec3 _normals;
out vec3 _worldPos;
out vec3 _cameraPos;
out vec2 _ssUVs;

void main()
{
	_texCoords = textureCoords;
	_normals = (vec4(normals.xyz, 1) * inverse(model)).rgb;
	_worldPos = (model * vec4(vertex, 1)).xyz;
	_ssUVs = gl_Position.xy/gl_Position.w;

	mat4 invView = inverse(view);
	_cameraPos = vec3(invView[3][0], invView[3][1], invView[3][2]);

	gl_Position = projection * view * model * vec4(vertex, 1);
}