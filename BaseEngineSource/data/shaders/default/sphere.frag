#version 330 core
#include "BEGlobal"
#include "Lighting"

in vec2 _texCoords;
in vec3 _normals;
in vec3 _worldPos;
in vec3 _cameraPos;

uniform sampler2D mainTexture;
uniform sampler2D distortTexture;

out vec4 FragColor;

void main()
{
	float bias = 1;
	float scale = 0.02;

	float v = dot(_normals, normalize(_cameraPos - _worldPos)); 
	v = clamp(1.0 - v, 0, 1);
	v = pow(v, 2);

	ivec2 texSize = textureSize(mainTexture, 0);

	vec2 uvs = gl_FragCoord.xy / global.screen.xy;

	vec2 distort = (texture(distortTexture, _texCoords).xy * 2.0) - 1.0;
	distort *= 0.05;

	vec3 c = mix(texture(mainTexture, uvs).rgb, vec3(0.094, 0.545, 0.768), v);
	FragColor = vec4(c, 1); 
}