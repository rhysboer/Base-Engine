#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 uvs; // Tex Coordinates
layout (location = 3) in vec3 tangent;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out vec3 _normals;
out vec3 _worldPos;
out vec2 _uvs;
out mat3 _tbn;

void main()
{
	vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
	vec3 B = normalize(vec3(model * vec4(cross(normals, tangent), 0.0f)));
	vec3 N = normalize(vec3(model * vec4(normals, 0.0f)));
	_tbn = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(vertex, 1);

	_normals = (vec4(normals.xyz, 1) * inverse(model)).rgb;
	_uvs = uvs;
	_worldPos = (model * vec4(vertex, 1)).xyz;
}