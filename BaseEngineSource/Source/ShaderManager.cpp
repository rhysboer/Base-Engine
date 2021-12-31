#include "ShaderManager.h"
#include <fstream>
#include "Logger.h"

namespace BE {
	std::vector<Shader*> ShaderManager::shaders = std::vector<Shader*>();
	std::unordered_map<const char*, int> ShaderManager::shaderIndexes = std::unordered_map<const char*, int>();
	unsigned int ShaderManager::shaderCount = 0;

	//std::map<std::string, Shader*> ShaderManager::graphicShaders = std::map<std::string, Shader*>();
	//std::map<std::string, Shader*> ShaderManager::computeShaders = std::map<std::string, Shader*>();

	//void ShaderManager::DestroyShaders() {
	//	// Destroy Graphic Shaders
	//	std::map<std::string, Shader*>::iterator iter = graphicShaders.begin();
	//	for(; iter != graphicShaders.end(); iter++) {
	//		delete iter->second;
	//		iter->second = nullptr;
	//	}
	//
	//	// Destroy Compute Shaders
	//	iter = computeShaders.begin();
	//	for(; iter != computeShaders.end(); iter++) {
	//		delete iter->second;
	//		iter->second = nullptr;
	//	}
	//}

	int ShaderManager::AddShaderFile(std::string name, std::string shaderPath, bool useGeometryShader) {
		return AddShaderFile(name.c_str(), (shaderPath + VERT_APPEND).c_str(), (shaderPath + FRAG_APPEND).c_str(), (useGeometryShader) ? (shaderPath + GEOM_APPEND).c_str() : nullptr);
	}

	int ShaderManager::AddShaderFile(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
		std::string vertSource, fragSource, geomSource;
		std::ifstream file;

		// Find if shader already exists
		if(shaderIndexes.find(name) != shaderIndexes.end()) {
			BE::Logger::Warning("ShaderManager: Name already exists in current list - ", name);
			return -1;
		}

		// VERTEX
		file.open(vertexPath);
		if(!file.is_open()) {
			BE::Logger::Error("ShaderManager: Failed to find file - ", vertexPath);
			file.close();
			return -1;
		}

		vertSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// FRAGMENT
		file.open(fragmentPath);
		if(!file.is_open()) {
			BE::Logger::Error("ShaderManager: Failed to find file - ", fragmentPath);
			file.close();
			return -1;
		}

		fragSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// GEOMETRY
		if(geometryPath != nullptr) {
			file.open(geometryPath);
			if(!file.is_open()) {
				BE::Logger::Error("ShaderManager: Failed to find file - ", geometryPath);
				file.close();
				return -1;
			}

			geomSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
		}

		return AddShaderSource(name, vertSource.c_str(), fragSource.c_str(), (geometryPath != nullptr) ? geomSource.c_str() : nullptr);
	}

	int ShaderManager::AddShaderSource(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
		int index = -1;

		// CREATE SHADER
		Shader* shader = new Shader();
		if(shader->LoadShader(vertexSource, fragmentSource, geometrySource)) {
			index = shaderCount;
			++shaderCount;

			shaders.push_back(shader);
			shaderIndexes.insert({ name, index });
		} else {
			delete shader;
		}

		return index;
	}

	Shader* ShaderManager::GetShader(const char* name) {
		auto iter = shaderIndexes.find(name);
		return (iter != shaderIndexes.end()) ? shaders[(*iter).second] : nullptr;
	}

	Shader* ShaderManager::GetShader(const int& shaderId) {
		assert(shaderId >= 0 && shaderId < shaders.size() && "ShaderManager: ShaderId out of range");
		return shaders[shaderId];
	}

	int ShaderManager::GetShaderId(const char* name) {
		auto iter = shaderIndexes.find(name);
		return (iter != shaderIndexes.end()) ? (*iter).second : -1;
	}

	/*
	Shader* ShaderManager::GetShader(const char* name, ShaderType type) {
		switch(type) {
			case ShaderManager::ShaderType::GRAPHIC:
			{
				std::map<std::string, Shader*>::iterator iter = graphicShaders.find(name);
				if(iter != graphicShaders.end())
					return (*iter).second;
				break;
			}
			case ShaderManager::ShaderType::COMPUTE:
			{
				std::map<std::string, Shader*>::iterator iter = computeShaders.find(name);
				if(iter != computeShaders.end())
					return (*iter).second;
				break;
			}
		}

		printf("Failed to find shader: %s!\n", name);
		return nullptr;
	}

	Shader* ShaderManager::AddShader(std::string name, std::string shaderPath, bool useGeometryShader) {
		return AddShader(name.c_str(), (shaderPath + ".vert").c_str(), (shaderPath + ".frag").c_str(), (useGeometryShader) ? (shaderPath + ".geom").c_str() : nullptr);
	}

	Shader* ShaderManager::AddShader(const char* name, const char* vertPath, const char* fragPath, const char* geomPath) {
		std::ifstream file;

		std::string vertSource;
		std::string fragSource;
		std::string geomSource;

		// Find if shader already exists
		std::map<std::string, Shader*>::iterator iter = graphicShaders.find(name);
		if(iter != graphicShaders.end())
			return (*iter).second;

		// VERTEX
		file.open(vertPath);
		if(!file.is_open()) {
			printf("Failed to load Vertex Shader: %s\n", vertPath);
			file.close();
			return nullptr;
		}

		vertSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// FRAGMENT
		file.open(fragPath);
		if(!file.is_open()) {
			printf("Failed to load Fragment Shader: %s\n", fragPath);
			file.close();
			return nullptr;
		}

		fragSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// GEOMETRY
		if(geomPath != nullptr) {
			file.open(geomPath);
			if(!file.is_open()) {
				printf("Failed to load Geometry Shader: %s\n", geomPath);
				file.close();
				return nullptr;
			}

			geomSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
		}

		// CREATE SHADER
		Shader* shader = new Shader();
		if(shader->LoadShader(vertSource, fragSource, geomSource)) {
			graphicShaders.insert(std::pair<const char*, Shader*>(name, shader));
			return shader;
		}

		return nullptr;
	}

    Shader* ShaderManager::AddShader(std::string name, std::string vertexSource, std::string fragmentSource, std::string geometrySource) {
		// CREATE SHADER
		Shader* shader = new Shader();
		if(shader->LoadShader(vertexSource, fragmentSource, geometrySource)) {
			graphicShaders.insert(std::pair<const char*, Shader*>(name.c_str(), shader));
			return shader;
		}

		return nullptr;
    }

	Shader* ShaderManager::AddComputeShader(const char* name, const char* compPath) {
		std::ifstream file;

		file.open(compPath);

		if(!file.is_open()) {
			printf("Failed to load Compute Shader: %s\n", compPath);
			file.close();
			return nullptr;
		}

		std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		Shader* shader = new Shader();
		if(shader->LoadShader(source)) {
			computeShaders.insert(std::pair<const char*, Shader*>(name, shader));
			return shader;
		}

		return nullptr;
	}
	*/
}