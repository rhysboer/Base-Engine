#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Shader.h"

// The standard all-in-one shader, textured, colour and lighting.
#define BE_SHADER_STANDARD "BE::Standard"
// A single colour, nonlit shader.
#define BE_SHADER_UNLIT_COLOUR "BE::UnlitColour"
// An unlit texture shader.
#define BE_SHADER_UNLIT_TEXTURE "BE::UnlitTexture"
// An unlit vextex colour shader.
#define BE_SHADER_UNLIT_VERTEX_COLOUR "BE::UnlitVertexColour"
// A flat simple texture shader with basic built-in lighting.
#define BE_SHADER_FLAT "BE::Flat"
#define BE_SHADER_SCREENQUAD "BE::ScreenQuad"

namespace BE {
	class Uniform;
	class ITexture;
	class Framebuffer;
	class Material {
	public:
		Material(Material*) = delete;
		Material(const Material&) = delete;

		Material();
		Material(const Shader const * shader);
		Material(const char* shaderName);
		Material(const PremadeShaders id);
		~Material();

		void SetUniform(const char* name, const int& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const float& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::vec2& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::vec3& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::vec4& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::mat4& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const ITexture* texture);

		void SetMainTexture(const ITexture* texture);
		ITexture* GetMainTexture() const;

		inline void SetDepthWrite(const bool& enable);
		void SetTransparent(const bool& isTransparent);
		bool IsTransparent() const { return isTransparent; }

		inline bool GetDepthWrite() const;
		inline int GetUniformCount() const;
		Uniform* GetUniform(const int& index) const;
		inline const Shader* const GetShader() const { return shader; }

		void Use() const;

	private:
		Uniform* GetUniform(const char* name) const;

		std::unordered_map<std::string, unsigned int> indexing = std::unordered_map<std::string, unsigned int>();
		std::vector<Uniform*> uniforms = std::vector<Uniform*>();

		Shader const* shader;
		bool isTransparent = false;
		bool depthWrite = true;
	};

	inline void Material::SetDepthWrite(const bool& enable)
	{
		depthWrite = enable;
	}

	inline bool Material::GetDepthWrite() const
	{
		return depthWrite;
	}

	inline int Material::GetUniformCount() const
	{
		return uniforms.size();
	}
}

