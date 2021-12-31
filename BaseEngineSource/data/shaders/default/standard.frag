#version 330 core

uniform vec3 colour;
uniform sampler2D mainTexture;

in vec3 _normals;
in vec2 _uvs;

out vec4 FragColor;

void main()
{
	vec3 sample = texture(mainTexture, _uvs).rgb;
	vec3 frag = sample * colour;
	FragColor = vec4(frag.rgb, 1); //vec4(_uvs.x, _uvs.y, 0, 1); 
}