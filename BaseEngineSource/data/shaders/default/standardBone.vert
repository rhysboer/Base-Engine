#version 330 core
layout (location = 0) in vec3 vertex; // Vertex Position
layout (location = 1) in vec3 normals; // Normals
layout (location = 2) in vec2 uvs; // Tex Coordinates
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec4 boneIds;
layout (location = 5) in vec4 weights; 

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;
//uniform mat4 boneOffsets[2];

uniform mat4 bone1;
uniform mat4 bone2;

out vec3 _normals;
out vec3 _worldPos;
out vec2 _uvs;
out mat3 _tbn;

void main()
{
	vec4 totalPositions = vec4(0.0);
	for(int i = 0; i < 2; i++)
	{
		int id = int(boneIds[i]);
		if(id < 0)
			continue;
		if(id >= 100){
			totalPositions = vec4(vertex, 1.0);
			break;
		}
		
		mat4 b;
		if(id == 0)
			b = (bone1);
		else
			b = (bone2);
		
		vec4 localPosition = b * vec4(vertex, 1.0); //b[boneIds[i]] * vec4(vertex, 1.0);
		totalPositions += localPosition * weights[i];
	}
	
	//totalPositions.xyz = vertex;
	totalPositions.w = 1;
	//totalPositions.z = boneIds[0] * 1;

	vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
	vec3 B = normalize(vec3(model * vec4(cross(normals, tangent), 0.0f)));
	vec3 N = normalize(vec3(model * vec4(normals, 0.0f)));
	_tbn = mat3(T, B, N);

	gl_Position = projection * view * model * totalPositions; //vec4(vertex, 1);

	_normals = normalize((vec4(normals.xyz, 1) * inverse(model)).rgb);
	_uvs = uvs;
	_worldPos = (model * vec4(vertex, 1)).xyz;
}