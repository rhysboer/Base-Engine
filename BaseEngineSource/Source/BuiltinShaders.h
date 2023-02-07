#pragma once

namespace BE::ShaderSource {
	// -------- MISC SHADERS START --------
#pragma region POST_PROCESSING
	static const char* VERT_POSTPROCESS = R"(
		#version 330 core
		layout (location = 0) in vec3 vertex; // Vertex Position
		layout (location = 1) in vec2 uvs; // Normals

		out vec2 _uvs;

		void main()
		{
			_uvs = uvs;
			gl_Position = vec4(vertex, 1);
		}
	)";

	static const char* FRAG_POSTPROCESS = R"(
		#version 330 core

		uniform sampler2D mainTexture;
		
		in vec2 _uvs;
		out vec4 FragColor;

		void main()
		{
			FragColor = texture2D(mainTexture, _uvs);
		}
	)";
#pragma endregion

#pragma region RENDER_TO_TEXTURE



#pragma endregion


	// -------- MISC SHADERS END --------





	// -------- SHADER SOURCES START --------
#pragma region STANDARD_SHADER
	static const char* VERT_STANDARD =
		R"(
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
		)";

	static const char* FRAG_STANDARD =
		R"(
			#version 330 core
			#include BEGlobal
			#include Lighting

			uniform vec3 colour;

			uniform sampler2D mainTexture;
			uniform sampler2D normTexture;
			uniform vec2 tiling;

			in vec3 _normals;
			in vec3 _worldPos;
			in vec2 _uvs;
			in mat3 _tbn;
			
			out vec4 FragColor;

			void main()
			{
				vec4 sample = texture(mainTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgba;
				
				
				if(alphaClip == true && sample.a < alphaClipThreshold)
					discard;

				vec3 normalSample = _tbn * normalize(texture(normTexture, fract(_uvs * max(tiling, vec2(1, 1)))).rgb * 2.0 - 1.0);
				//normalSample.xy = normalize(2 * normalSample.xy);
	
				vec3 ambient = lights.ambient * sample.rgb;
				vec3 frag = (ambient + GetLight(normalSample, _worldPos)) * sample.rgb;
				
				float alpha = 1;
				if(isTransparent)
					alpha = sample.a;

				FragColor = vec4(frag, alpha);
			}

		)";
#pragma endregion

#pragma region UNLIT_SHADER
	static const char* FRAG_UNLIT =
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
	static const char* FRAG_FLAT =
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
#pragma region SHADER_HEADERS

	static const char* HEADER_NAME_BEGLOBAL = "BEGlobal";
	static const char* HEADER_SRC_BEGLOBAL =
		R"(
			uniform bool alphaClip;
			uniform bool isTransparent;
			uniform float alphaClipThreshold;

			layout (std140) uniform Common
			{
				vec4 screen; 
				vec4 time;
			} global;
		)";

	static const char* HEADER_NAME_LIGHTING = "Lighting";
	static const char* HEADER_SCR_LIGHTING = 
		R"(
			#define MAX_LIGHTS 16	

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

			vec3 GetLight(const vec3 a_normal, const vec3 a_worldPos)
			{
				vec3 l = vec3(0.2, 0.2, 0.2);

				int size = int(lights.count);
				for(int i = 0; i < size; i++)
				{
					int type = int(lights.lights[i].direction.w);
						
					switch(type) // Light Type
					{
						case 0: // Directional
						{
							l += lights.lights[i].colour.rgb * clamp(dot(a_normal, lights.lights[i].direction.xyz), 0.0, 1.0); 
							break;
						}
						case 1: // Spot
						{
							vec3 v = normalize(a_worldPos - lights.lights[i].position.xyz);
							float d = dot(v, -lights.lights[i].direction.xyz);

							l += lights.lights[i].colour.rgb * smoothstep(lights.lights[i].data.y, lights.lights[i].data.z, d) * clamp(-dot(a_normal, v), 0.0, 1.0);
							break;
						}
						case 2: // Point
						{
							vec3 v = normalize(a_worldPos - lights.lights[i].position.xyz);
							float d = clamp(-dot(v, a_normal), 0.0, 1.0);

							l += lights.lights[i].colour.rgb * (smoothstep(0, 1, vec3(clamp(lights.lights[i].data.x - distance(lights.lights[i].position.xyz, a_worldPos), 0, lights.lights[i].data.x))) * d);
							break;
						}	
					}
				}

				return l;		
			}
			
		)";
#pragma endregion
	// -------- SHADER HEADERS END --------
}

