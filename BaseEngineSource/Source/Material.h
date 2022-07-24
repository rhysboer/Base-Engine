#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

namespace BE {
	class Shader;
	class Uniform;
	class ITexture;
	class Material {
	public:

		Material(const Shader const * shader);
		Material(const char* shaderName);
		Material(const int& shaderId);
		~Material();

		void SetUniform(const char* name, const int& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const float& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::vec2& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::vec3& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const glm::mat4& value, const unsigned int& size = 1);
		void SetUniform(const char* name, const ITexture* texture);

		void SetTransparent(const bool& isTransparent);
		bool IsTransparent() const { return isTransparent; }

		void UpdateShaders();

	private:
		Uniform* GetUniform(const char* name) const;

		std::unordered_map<std::string, unsigned int> indexing = std::unordered_map<std::string, unsigned int>();
		std::vector<Uniform*> uniforms = std::vector<Uniform*>();

		Shader const* shader;
		bool isTransparent;
	};
}

