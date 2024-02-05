#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Shader.h"


#define BE_SHADER_SHADOW "BE::ShadowMap"

// Standard Shader Uniforms
#define BE_SHADER_UNI_ALPHACLIP "alphaClip"
#define BE_SHADER_UNI_ALPHACLIP_THRES "alphaClipThreshold"
#define BE_SHADER_UNI_TRANSPARENT "isTransparent"

namespace BE {
	class ITexture;
	class BaseEngine;
	class ShaderManager {
		friend Shader;
		friend BaseEngine;
	public:
		//enum class ShaderType {
		//	GRAPHIC, // Vertex, Fragment and, Geometry Shaders
		//	COMPUTE  // Compute Shaders
		//};

		//static int AddShaderFile(std::string name, std::string shaderPath, bool useGeometryShader = false);
		//static int AddShaderFile(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		//static int AddShaderSource(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

		static void AddShaderHeader(const char* headerName, const char* header);

		// TODO
		//static Shader* AddComputeShader(const char* name, const char* compPath);
		
		static ITexture* GetGlobalTexture(const char* name);

		static Shader* GetPremadeShader(const PremadeShaders& shader);
		static Shader* GetShader(const char* name);

	private:
		static void Init();
		static void Destroy();

		static void RegisterShader(Shader* shader);
		static void DeregisterShader(Shader* shader);

		static void TryAddHeaders(std::string& source);

		static std::vector<Shader*> premadeShaders;
		static std::unordered_map<std::string, Shader*> shaders;

		static std::unordered_map<std::string, std::string> shaderHeaders;
		static std::unordered_map<std::string, ITexture*> globalTextures;
	};
}