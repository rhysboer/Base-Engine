#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Shader.h"

#define FRAG_APPEND ".frag"
#define VERT_APPEND ".vert"
#define GEOM_APPEND ".geom"

// The standard all-in-one shader, textured, colour and lighting.
#define BE_SHADER_STANDARD "BE::Standard"
// A single colour, nonlit shader.
#define BE_SHADER_UNLIT "BE::UnlitColour"
// A flat simple texture shader with basic built-in lighting.
#define BE_SHADER_FLAT "BE::Flat"

// Standard Shader Uniforms
#define BE_SHADER_UNI_ALPHACLIP "alphaClip"
#define BE_SHADER_UNI_ALPHACLIP_THRES "alphaClipThreshold"
#define BE_SHADER_UNI_TRANSPARENT "isTransparent"

namespace BE {
	class BaseEngine;
	class ShaderManager {
		friend BaseEngine;
	public:
		enum class ShaderType {
			GRAPHIC, // Vertex, Fragment and, Geometry Shaders
			COMPUTE  // Compute Shaders
		};

		// TODO
		static void DestroyShaders();

		static int AddShaderFile(std::string name, std::string shaderPath, bool useGeometryShader = false);
		static int AddShaderFile(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		static int AddShaderSource(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

		static void AddShaderHeader(const char* headerName, const char* header);

		// TODO
		//static Shader* AddComputeShader(const char* name, const char* compPath);
		
		static Shader* GetShader(const char* name);
		static Shader* GetShader(const int& shaderId);

		static int GetShaderId(const char* name);

	private:

		static void InitBaseShaders();
		static void TryAddHeaders(std::string& source);


		static std::vector<Shader*> shaders;
		static std::unordered_map<const char*, int> shaderIndexes;
		static std::unordered_map<std::string, std::string> shaderHeaders;

		static unsigned int shaderCount;
	};
}