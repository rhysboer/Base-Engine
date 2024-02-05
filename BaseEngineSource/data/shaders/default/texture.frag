#version 330 core
#include "BEGlobal"
#include "Lighting"

uniform vec3 colour;

uniform sampler2D mainTexture;

in vec3 _normals;
in vec3 _worldPos;
in vec2 _uvs;
in mat3 _tbn;

out vec4 FragColor;

void main()
{
	vec4 sample = texture(BE_shadowDepthMap, _uvs).rgba;
	FragColor = sample;
}