#include "ShaderManager.h"
#include <fstream>
#include "Logging.h"
#include "BuiltinShaders.h"
//#include "ITexture.h"

#include "Material.h"

namespace BE {
	//std::vector<Shader*> ShaderManager::shaders = std::vector<Shader*>();
	//std::unordered_map<std::string, int> ShaderManager::shaderIndexes = std::unordered_map<std::string, int>();
	std::vector<Shader*> ShaderManager::premadeShaders = std::vector<Shader*>();
	std::unordered_map<std::string, Shader*> ShaderManager::shaders = std::unordered_map<std::string, Shader*>();
	std::unordered_map<std::string, std::string> ShaderManager::shaderHeaders = std::unordered_map<std::string, std::string>();
	std::unordered_map<std::string, ITexture*> ShaderManager::globalTextures = std::unordered_map<std::string, ITexture*>();

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

	//int ShaderManager::AddShaderFile(std::string name, std::string shaderPath, bool useGeometryShader) {
	//	return AddShaderFile(name.c_str(), (shaderPath + VERT_APPEND).c_str(), (shaderPath + FRAG_APPEND).c_str(), (useGeometryShader) ? (shaderPath + GEOM_APPEND).c_str() : nullptr);
	//}
	//
	//int ShaderManager::AddShaderFile(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	//	std::string vertSource, fragSource, geomSource;
	//	std::ifstream file;
	//
	//	// Find if shader already exists
	//	if(shaders.find(name) != shaders.end()) { //shaderIndexes.find(name) != shaderIndexes.end()) {
	//		BE_ERROR("ShaderManager: Name already exists in current list - %s", name);
	//		return -1;
	//	}
	//
	//	// VERTEX
	//	file.open(vertexPath);
	//	if(!file.is_open()) {
	//		BE_WARNING("ShaderManager: Failed to find file - %s", vertexPath);
	//		file.close();
	//		return -1;
	//	}
	//
	//	vertSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	//	file.close();
	//
	//	// FRAGMENT
	//	file.open(fragmentPath);
	//	if(!file.is_open()) {
	//		BE_ERROR("ShaderManager: Failed to find file - %s", fragmentPath);
	//		file.close();
	//		return -1;
	//	}
	//
	//	fragSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	//	file.close();
	//
	//	TryAddHeaders(fragSource);
	//
	//	// GEOMETRY
	//	if(geometryPath != nullptr) {
	//		file.open(geometryPath);
	//		if(!file.is_open()) {
	//			BE_ERROR("ShaderManager: Failed to find file - ", geometryPath);
	//			file.close();
	//			return -1;
	//		}
	//
	//		geomSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	//		file.close();
	//	}
	//
	//	return AddShaderSource(name, vertSource.c_str(), fragSource.c_str(), (geometryPath != nullptr) ? geomSource.c_str() : nullptr);
	//}

	//int ShaderManager::AddShaderSource(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
	//	int index = -1;
	//
	//	std::string fragString = std::string(fragmentSource);
	//	TryAddHeaders(fragString);
	//
	//	// CREATE SHADER
	//	Shader* shader = new Shader();
	//	if(shader->LoadShader(vertexSource, fragString.c_str(), geometrySource)) {
	//		index = shaderCount;
	//		++shaderCount;
	//
	//		shaders.emplace(name, shader);
	//		//shaders.push_back(shader);
	//		//shaderIndexes.insert({ name, index });
	//	} else {
	//		delete shader;
	//	}
	//
	//	return index;
	//}

	void ShaderManager::AddShaderHeader(const char* headerName, const char* header)
	{
		// TODO: should change shaderHeader to use std::String instead of const char*
		shaderHeaders.emplace(headerName, header);
	}

	ITexture* ShaderManager::GetGlobalTexture(const char* name)
	{
		auto iter = globalTextures.find(name);
		return (iter != globalTextures.end()) ? (*iter).second : nullptr;
	}

	Shader* ShaderManager::GetPremadeShader(const PremadeShaders& shader)
	{
		if (shader >= PremadeShaders::_COUNT)
			return nullptr;

		return premadeShaders[(int)shader];
	}

	Shader* ShaderManager::GetShader(const char* name) {
		auto iter = shaders.find(name);
		return (iter != shaders.end()) ? (*iter).second : nullptr;
	}

	//Shader* ShaderManager::GetShader(const int& shaderId) {
	//	assert(shaderId >= 0 && shaderId < shaders.size() && "ShaderManager: ShaderId out of range");
	//	return shaders[shaderId];
	//}

	//int ShaderManager::GetShaderId(const char* name) {
	//	auto iter = shaders.find(name);
	//	return (iter != shaders.end()) ? (*iter).second : 
	//
	//	//auto iter = shaderIndexes.find(name);
	//	//return (iter != shaderIndexes.end()) ? (*iter).second : -1;
	//}

	void ShaderManager::TryAddHeaders(std::string& source)
	{
		int offset = 0;
		int s = 0;
		do {
			s = source.find("#include", offset);
			offset += s;

			if (s > 0) {
				int start = source.find("\"", offset);
				int end = source.find("\"", start + 1);

				if (end != std::string::npos) {
					auto name = source.substr(start + 1, end - start - 1); //source.substr(offset + 9, end - offset - 9);
					auto iter = shaderHeaders.find(name.c_str());

					if (iter != shaderHeaders.end()) {
						source.replace(offset, end - offset + 1, (*iter).second);
						offset = 0;
					} else {
						source.replace(offset, end - offset + 1, "");
					}
				}
			}

		} while (s != std::string::npos);
	}

	void ShaderManager::RegisterShader(Shader* shader)
	{
		if (shaders.find(shader->name) != shaders.end())
			return;
		shaders.emplace(shader->name, shader);
	}

	void ShaderManager::DeregisterShader(Shader* shader)
	{
		// This function should only be called when a Shader class is being deconstructed.
		// No need to delete the pointer here.

		shaders.erase(shader->name);
	}

	void ShaderManager::Init() {
		// Create Headers
		AddShaderHeader(ShaderSource::HEADER_NAME_BEGLOBAL, ShaderSource::HEADER_SRC_BEGLOBAL);
		AddShaderHeader(ShaderSource::HEADER_NAME_LIGHTING, ShaderSource::HEADER_SCR_LIGHTING);

		// Create Engine Shaders
		premadeShaders = std::vector<Shader*>(PremadeShaders::_COUNT);
		premadeShaders[(int)PremadeShaders::STANDARD] = Shader::CreateShader(BE_SHADER_STANDARD, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_STANDARD);
		premadeShaders[(int)PremadeShaders::UNLIT_COLOUR] = Shader::CreateShader(BE_SHADER_UNLIT_COLOUR, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT_COLOUR);
		premadeShaders[(int)PremadeShaders::UNLIT_TEXTURE] = Shader::CreateShader(BE_SHADER_UNLIT_TEXTURE, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT_TEXTURE);
		premadeShaders[(int)PremadeShaders::FLAT] = Shader::CreateShader(BE_SHADER_FLAT, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_FLAT);
		//premadeShaders[(int)PremadeShaders::UNLIT_VERTEX_COLOUR] = (new Shader(BE_SHADER_UNLIT_VERTEX_COLOUR, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT_VERTEX_COLOUR)); // TODO
		premadeShaders[(int)PremadeShaders::SCREEN] = Shader::CreateShader(BE_SHADER_SCREENQUAD, ShaderSource::VERT_POSTPROCESS, ShaderSource::FRAG_POSTPROCESS);
		premadeShaders[(int)PremadeShaders::_ENGINE_SHADOW] = Shader::CreateShader(BE_SHADER_SHADOW, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_POSTPROCESS);

		//AddShaderSource(BE_SHADER_STANDARD, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_STANDARD);
		//AddShaderSource(BE_SHADER_UNLIT_COLOUR, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT_COLOUR);
		//AddShaderSource(BE_SHADER_UNLIT_TEXTURE, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_UNLIT_TEXTURE);
		////AddShaderSource(BE_SHADER_UNLIT_VERTEX_COLOUR, ShaderSource::VERT_UNLIT_VERTEX_COLOUR, ShaderSource::FRAG_UNLIT_VERTEX_COLOUR);
		//AddShaderSource(BE_SHADER_FLAT, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_FLAT);
		//AddShaderSource(BE_SHADER_SCREENQUAD, ShaderSource::VERT_POSTPROCESS, ShaderSource::FRAG_POSTPROCESS);
		//AddShaderSource(BE_SHADER_SHADOW, ShaderSource::VERT_STANDARD, ShaderSource::FRAG_SHADOWMAP);
    }

	void ShaderManager::Destroy()
	{

	}
}