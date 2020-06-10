#include "ShaderManager.h"

std::map<const char*, Shader*> ShaderManager::graphicShaders = std::map<const char*, Shader*>();
std::map<const char*, Shader*> ShaderManager::computeShaders = std::map<const char*, Shader*>();

void ShaderManager::DestroyShaders() {
	// Destroy Graphic Shaders
	std::map<const char*, Shader*>::iterator iter = graphicShaders.begin();
	for(; iter != graphicShaders.end(); iter++) {
		delete iter->second;
		iter->second = nullptr;
	}

	// Destroy Compute Shaders
	iter = computeShaders.begin();
	for(; iter != computeShaders.end(); iter++) {
		delete iter->second;
		iter->second = nullptr;
	}
}

Shader* ShaderManager::GetShader(const char* name, ShaderType type) {
	switch(type) {
		case ShaderManager::ShaderType::GRAPHIC:
		{
			std::map<const char*, Shader*>::iterator iter = graphicShaders.find(name);
			if(iter != graphicShaders.end())
				return (*iter).second;
			break;
		}
		case ShaderManager::ShaderType::COMPUTE:
		{
			std::map<const char*, Shader*>::iterator iter = computeShaders.find(name);
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