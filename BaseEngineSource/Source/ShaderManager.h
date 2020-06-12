#pragma once
#include <fstream>
#include <map>
#include <string>
#include "Shader.h"

class ShaderManager {
public:
	enum class ShaderType {
		GRAPHIC, // Vertex & Fragment and Geometry Shaders
		COMPUTE  // Compute Shaders
	};

	static void DestroyShaders();
	
	static Shader* AddShader(std::string name, std::string shaderPath, bool useGeometryShader = false);
	static Shader* AddShader(const char* name, const char* vertPath, const char* fragPath, const char* geomPath = nullptr);
	static Shader* AddComputeShader(const char* name, const char* compPath);

	static Shader* GetShader(const char* name, ShaderType type = ShaderType::GRAPHIC);

private:

	static std::map<std::string, Shader*> graphicShaders;
	static std::map<std::string, Shader*> computeShaders;
};

