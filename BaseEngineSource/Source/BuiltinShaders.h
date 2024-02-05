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

				_normals = normalize((vec4(normals.xyz, 1) * inverse(model)).rgb);
				_uvs = uvs;
				_worldPos = (model * vec4(vertex, 1)).xyz;
			}
		)";

	static const char* FRAG_STANDARD =
		R"(
			#version 330 core
			#include "BEGlobal"
			#include "Lighting"

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
				vec3 frag = (ambient + /*GetLight(normalSample, _worldPos)*/ CalculateLighting(_worldPos, normalSample, _normals) ) * sample.rgb;// * GetShadow(_worldPos, _normals);
				
				float alpha = 1;
				if(isTransparent)
					alpha = sample.a;

				//vec3 c = CalculateLighting(_worldPos, normalSample, _normals);
				//FragColor = vec4(c, 1);
				//float l = /*GetShadow(_worldPos, _normals); */ //GetLight(normalSample, _worldPos, c);
				//FragColor = vec4(l, l, l, 1);
				//FragColor = vec4(CalculateLighting(_worldPos, normalSample, _normals), 1);
				FragColor = vec4(frag, alpha);
			}

		)";
#pragma endregion

#pragma region UNLIT_SHADER_COLOUR
	static const char* FRAG_UNLIT_COLOUR =
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

#pragma region UNLIT_SHADER_TEXTURE
	static const char* FRAG_UNLIT_TEXTURE =
		R"(
			#version 330 core
			uniform sampler2D mainTexture;
			
			in vec3 _normals;
			in vec2 _uvs;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = texture(mainTexture, _uvs).rgba;
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

#pragma region UNLIT_SHADER_VERTEX_COLOUR
	/*
	static const char* VERT_UNLIT_VERTEX_COLOUR =
		R"(
			#version 330 core
			layout (location = 0) in vec3 vertex; // Vertex Position
			layout (location = 1) in vec3 normals; // Normals
			layout (location = 2) in vec2 uvs; // Tex Coordinates
			layout (location = 3) in vec3 tangent;
			layout (location = 4) in vec3 colour;
			
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
			out vec3 _colour;
			
			void main()
			{
				vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
				vec3 B = normalize(vec3(model * vec4(cross(normals, tangent), 0.0f)));
				vec3 N = normalize(vec3(model * vec4(normals, 0.0f)));
				_tbn = mat3(T, B, N);
			
				gl_Position = projection * view * model * vec4(vertex, 1);
			
				_normals = normalize((vec4(normals.xyz, 1) * inverse(model)).rgb);
				_uvs = uvs;
				_worldPos = (model * vec4(vertex, 1)).xyz;
				_colour = colour;
			}
		)";

	static const char* FRAG_UNLIT_VERTEX_COLOUR =
		R"(
			#version 330 core
			uniform sampler2D mainTexture;
			
			in vec3 _colour;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = float4(_colour, 1.0);
			}
		)";
		*/
#pragma endregion


#pragma region SHADOW_MAP
	static const char* FRAG_SHADOWMAP =
		R"(
			#version 330 core
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(1, 1, 1, 1); 
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

			uniform sampler2D BE_shadowDepthMap;

			layout (std140) uniform ShadowMap
			{
				mat4 lightSpace;
			} shadowMap;

			float GetLight(const vec3 pixelNormal, const vec3 surfaceNormal, const vec3 worldPos, out vec3 outColour)
			{
				int type = 0;
				float l = 0;
				float intensity = 0.0;
				outColour = vec3(0);

				int size = int(lights.count);
				for(int i = 0; i < size; i++)
				{
					type = int(lights.lights[i].direction.w);			

					switch(type) // Light Type
					{
						case 0: // Directional
						{
							l = clamp(dot(pixelNormal, lights.lights[i].direction.xyz), 0.0, 1.0); 
							//outColour += lights.lights[i].colour.rgb * l;
							break;
						}
						case 1: // Spot
						{
							vec3 v = normalize(worldPos - lights.lights[i].position.xyz);
							float d = dot(v, -lights.lights[i].direction.xyz);
				
							//intensity += smoothstep(lights.lights[i].data.y, lights.lights[i].data.z, d) * clamp(-dot(pixelNormal, v), 0.0, 1.0);
							break;
						}
						case 2: // Point
						{
							vec3 v = normalize(worldPos - lights.lights[i].position.xyz);
							float d = clamp(-dot(v, pixelNormal), 0.0, 1.0);
				
							//intensity += (smoothstep(0, 1, vec3(clamp(lights.lights[i].data.x - distance(lights.lights[i].position.xyz, worldPos), 0, lights.lights[i].data.x))) * d);
							break;
						}	
					}

					intensity += l;
					l = 0;
				}

				outColour = vec3(1);
				return 1.0 - pow(1.0 - intensity, 3);// intensity; //clamp(1.0 - pow(1.0 - intensity, 80), 0.2, 1);		
			}

			float GetShadow(vec3 a_worldPos, vec3 a_normals)
			{
				vec4 coords = shadowMap.lightSpace * vec4(a_worldPos + (a_normals * 0.002), 1.0);
				vec3 projCoords = coords.xyz / coords.w;
				projCoords = projCoords * 0.5 + 0.5;

				if(dot(a_normals, lights.lights[0].direction.xyz) < 0 || projCoords.z > 1.0)
					return 1;

				vec2 texel = 1.0 / textureSize(BE_shadowDepthMap, 0);
				float bias = 0;//0.0005;

				float shadow = 0.0;
				for(int x = -1; x <= 1; x++){
					for(int y = -1; y <= 1; y++){
						float shadowDepth = texture(BE_shadowDepthMap, projCoords.xy + (vec2(x, y) * texel)).r;
						float currentDepth = projCoords.z;		

						if(currentDepth - bias > shadowDepth)
							shadow += 1;	
					}
				}

				shadow /= 9.0;

				//float shadow = currentDepth - bias > shadowDepth ? 0.0 : 1.0;

				return 1.0 - shadow; //max(0.0, shadow);// dot(a_normals, lights.lights[0].direction.xyz);
			}

			vec3 CalculateLighting(const vec3 pixelWorldPos, const vec3 pixelNormal, const vec3 surfaceNormal)
			{
				vec3 lightColour = vec3(0);
				float light = GetLight(pixelNormal, surfaceNormal, pixelWorldPos, lightColour);				
				float shadow = GetShadow(pixelWorldPos, surfaceNormal);

				vec3 output = clamp(vec3(shadow * light * lightColour), vec3(0.2), vec3(10.0)); 
				
				return output;
			}
		)";
#pragma endregion
	// -------- SHADER HEADERS END --------
}