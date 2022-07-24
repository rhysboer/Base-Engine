#pragma once

namespace BE::ShaderSource {
	// -------- SHADER SOURCES START --------
#pragma region STANDARD_SHADER
	const char* VERT_STANDARD =
		R"(
			#version 330 core
			layout (location = 0) in vec3 vertex; // Vertex Position
			layout (location = 1) in vec3 normals; // Normals
			layout (location = 2) in vec2 uvs; // Tex Coordinates
			layout (location = 3) in vec3 tangent;
			
			layout (std140) uniform Camera
			{
				mat4 projectionView;
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

				gl_Position = projectionView * model * vec4(vertex, 1);

				_normals = (vec4(normals.xyz, 1) * inverse(model)).rgb;
				_uvs = uvs;
				_worldPos = (model * vec4(vertex, 1)).xyz;
			}
		)";

	const char* FRAG_STANDARD =
		R"(
			#version 330 core
			#include BEGlobal
			#define MAX_LIGHTS 16	
			uniform vec3 colour;

			uniform sampler2D mainTexture;
			uniform sampler2D normTexture;
			uniform vec2 tiling;
			
			struct Light
			{
				vec4 colour; // XYZ=Colour,W=Intensity
				vec4 direction; // XYZ=Direction,W=Type
				vec4 position; // XYZ=Position,W=???
				vec4 data; // X=Length,Y=InnerRadius,Z=OuterRadius,W=???				
			};

			layout (std140) uniform Lights
			{
				Light lights[MAX_LIGHTS];
				float ambient;
				int count;
			} lights;

			in vec3 _normals;
			in vec3 _worldPos;
			in vec2 _uvs;
			in mat3 _tbn;
			
			out vec4 FragColor;
			
			vec3 GetLight()
			{
				vec3 l = vec3(0.2, 0.2, 0.2);

				int size = int(lights.count);
				for(int i = 0; i < size; i++)
				{
					int type = int(lights.lights[i].direction.w);
					vec3 normalSample = texture(normTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgb * 2.0 - 1.0;
					normalSample = normalize(_tbn * normalSample);
						
					switch(type) // Light Type
					{
						case 0: // Directional
							l += lights.lights[i].colour.rgb * clamp(dot(normalSample, lights.lights[i].direction.xyz), 0.0, 1.0); 
							break;
						case 1: // Spot
							vec3 v = normalize(_worldPos - lights.lights[i].position.xyz);
							float d = dot(v, -lights.lights[i].direction.xyz);

							l += lights.lights[i].colour.rgb * smoothstep(lights.lights[i].data.y, lights.lights[i].data.z, d) * clamp(-dot(normalSample, v), 0.0, 1.0);
							break;
						case 2: // Point
							//l += clamp(-dot(normalSample, v), 0.0, 1.0);
							break;
					}
				}

				return l;		
			}

			void main()
			{
				vec4 sample = texture(mainTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgba;
				
				
				if(alphaClip == true && sample.a < alphaClipThreshold)
					discard;

				vec3 ambient = lights.ambient * sample.rgb;
				vec3 frag = (ambient + GetLight()) * sample.rgb;
				
				float alpha = 1;
				if(isTransparent)
					alpha = sample.a;

				//frag = vec3(_uvs.x, _uvs.y, 0);
				FragColor = vec4(sample.rgb, alpha); 
			}

		)";
#pragma endregion

#pragma region UNLIT_SHADER
	const char* FRAG_UNLIT =
		R"(
			#version 330 core
			uniform vec3 colour;
			
			in vec3 _normals;
			in vec2 _uvs;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(colour.rgb, 1);
			}
		)";
#pragma endregion

#pragma region FLAT_SHADER
	const char* FRAG_FLAT =
		R"(
			#version 330 core
			uniform vec3 colour;
			uniform vec3 sunDir;
			uniform sampler2D mainTexture;
			
			in vec3 _normals;
			in vec2 _uvs;
			
			out vec4 FragColor;
			
			vec3 sunCol = vec3(1.0, 1.0, 1.0);

			void main()
			{
				float sun = clamp(dot(_normals, -sunDir), 0.0, 0.9);
				vec3 sample = texture2D(mainTexture, _uvs).rgb * colour;
				FragColor = vec4(mix(sample, sunCol, sun), 1) * 0.0;
				FragColor = vec4(_uvs.xyy, 1);
			}
		)";
#pragma endregion
	// -------- SHADER SOURCES END --------

	// -------- SHADER HEADERS START --------

	const char* HEADER_NAME_BEGLOBAL = "BEGlobal";
	const char* HEADER_SOURCE_BEGLOBAL = 
		R"(
			uniform bool alphaClip;
			uniform bool isTransparent;
			uniform float alphaClipThreshold;
		)";

	// -------- SHADER HEADERS END --------
}

