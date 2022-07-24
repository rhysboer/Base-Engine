#include "ShaderManager.h"
#include <fstream>
#include "Logging.h"
#include "BuiltinShaders.h"

namespace BE {
	std::vector<Shader*> ShaderManager::shaders = std::vector<Shader*>();
	std::unordered_map<const char*, int> ShaderManager::shaderIndexes = std::unordered_map<const char*, int>();
	std::unordered_map<std::string, std::string> ShaderManager::shaderHeaders = std::unordered_map<std::string, std::string>();

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
			BE::Logging::Warning("ShaderManager: Name already exists in current list - ", name);
			return -1;
		}

		// VERTEX
		file.open(vertexPath);
		if(!file.is_open()) {
			BE::Logging::Error("ShaderManager: Failed to find file - ", vertexPath);
			file.close();
			return -1;
		}

		vertSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// FRAGMENT
		file.open(fragmentPath);
		if(!file.is_open()) {
			BE::Logging::Error("ShaderManager: Failed to find file - ", fragmentPath);
			file.close();
			return -1;
		}

		fragSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		TryAddHeaders(fragSource);

		// GEOMETRY
		if(geometryPath != nullptr) {
			file.open(geometryPath);
			if(!file.is_open()) {
				BE::Logging::Error("ShaderManager: Failed to find file - ", geometryPath);
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

		std::string fragString = std::string(fragmentSource);
		TryAddHeaders(fragString);

		// CREATE SHADER
		Shader* shader = new Shader();
		if(shader->LoadShader(vertexSource, fragString.c_str(), geometrySource)) {
			index = shaderCount;
			++shaderCount;

			shaders.push_back(shader);
			shaderIndexes.insert({ name, index });
		} else {
			delete shader;
		}

		return index;
	}

	void ShaderManager::AddShaderHeader(const char* headerName, const char* header)
	{
		// TODO: should change shaderHeader to use std::String instead of const char*
		shaderHeaders.emplace(headerName, header);
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

	void ShaderManager::TryAddHeaders(std::string& source)
	{
		int offset = 0;
		int s = 0;
		do {
			s = source.find("#include", offset);
			offset += s;

			if (s > 0) {
				int end = source.find("\n", offset);
				if (end != std::string::npos) {
					auto name = source.substr(offset + 9, end - offset - 9);
					auto iter = shaderHeaders.find(name.c_str());

					if (iter != shaderHeaders.end()) {
						source.replace(offset, end - offset, (*iter).second);
					}
					else {
						source.replace(offset, end - offset, "");
					}
				}
			}

		} while (s != std::string::npos);
	}

    void ShaderManager::InitBaseShaders() {
		AddShaderHeader(ShaderSource::HEADER_NAME_BEGLOBAL, ShaderSource::HEADER_SOURCE_BEGLOBAL);

		AddShaderSource(BE_SHADER_STANDARD, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_STANDARD);
		AddShaderSource(BE_SHADER_UNLIT, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT);
		AddShaderSource(BE_SHADER_FLAT, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_FLAT);
    }
}