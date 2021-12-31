#pragma once
#include <unordered_map>
#include <string>

#include <vector>

#include "Shader.h"

#define FRAG_APPEND ".frag"
#define VERT_APPEND ".vert"
#define GEOM_APPEND ".geom"

namespace BE {
	class ShaderManager {
	public:
		enum class ShaderType {
			GRAPHIC, // Vertex, Fragment and, Geometry Shaders
			COMPUTE  // Compute Shaders
		};

		static void DestroyShaders();


		static int AddShaderFile(std::string name, std::string shaderPath, bool useGeometryShader = false);
		static int AddShaderFile(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

		static int AddShaderSource(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

		// TODO
		//static Shader* AddComputeShader(const char* name, const char* compPath);
		
		static Shader* GetShader(const char* name);
		static Shader* GetShader(const int& shaderId);

		static int GetShaderId(const char* name);

	private:

		static std::vector<Shader*> shaders;
		static std::unordered_map<const char*, int> shaderIndexes;
		static unsigned int shaderCount;
	};
}