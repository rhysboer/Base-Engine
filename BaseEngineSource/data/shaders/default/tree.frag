#version 330
#include "BEGlobal"
#include "Lighting"

uniform vec3 colour;

uniform sampler2D mainTexture;
uniform sampler2D normTexture;
uniform sampler2D opacTexture;
uniform vec2 tiling;

in vec3 _normals;
in vec3 _worldPos;
in vec2 _uvs;
in mat3 _tbn;

out vec4 FragColor;

void main()
{
	vec4 sample = texture(mainTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgba;

	float alpha = texture(opacTexture, fract(_uvs * max(tiling, vec2(1, 1)))).r;
	if(alpha < 0.5)
		discard;

	vec3 normalSample = texture(normTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgb * 2.0 - 1.0;
	normalSample = normalize(_tbn * normalize(normalSample));

	normalSample = (gl_FrontFacing) ? normalSample : -normalSample;

	vec3 ambient = lights.ambient * sample.rgb;
	vec3 frag = (ambient + GetLight(normalSample, _worldPos)) * sample.rgb;

	FragColor = vec4(frag, 1);
}
