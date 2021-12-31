#include "Material.h"
#include "Uniform.h"
#include "ShaderManager.h"
#include "UniformBuffer.h"
#include <glad/glad.h>

namespace BE {
	Material::Material(const char* shaderName) : Material(ShaderManager::GetShader(shaderName)) {}
	Material::Material(const int& shaderId) : Material(ShaderManager::GetShader(shaderId)) {}
	Material::Material(const Shader const * shader) : shader(shader) {
		int uniformCount = 0;
		
		glGetProgramiv(shader->ProgramID(), GL_ACTIVE_UNIFORMS, &uniformCount);
		int name_len = -1, num = -1, location = -1;
		GLenum type = GL_ZERO;
		for(int i = 0; i < uniformCount; i++) {
			char name[100];
			glGetActiveUniform(shader->ProgramID(), GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
			name[name_len] = 0;

			if(glGetUniformLocation(shader->ProgramID(), name) == -1)
				continue;

			Uniform* uniform = Uniform::CreateUniform((UniformType)type, name, num);
			indexing.emplace(name, uniforms.size());
			uniforms.push_back(uniform);
			
		}

		UniformBuffer::CreateUniformBufferFromShader(shader->ProgramID());
	}

	Material::~Material() {
		// TODO: Delete Materials
	}

	void Material::SetUniform(const char* name, const glm::mat4& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		assert(uniform != nullptr);

		uniform->SetValue((void*)&value[0][0], 16);
	}

	void Material::SetUniform(const char* name, const glm::vec3& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		assert(uniform != nullptr);

		uniform->SetValue((void*)&value[0], 3);
	}

	void Material::SetUniform(const char* name, const int* value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		assert(uniform != nullptr);

		uniform->SetValue(value, size);
	}

	void Material::SetUniform(const char* name, const float* value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		assert(uniform != nullptr);

		uniform->SetValue(value, size);
	}

	void Material::UpdateShaders() {
		shader->UseProgram();

		for(int i = 0; i < uniforms.size(); i++)
			shader->SetUniform(*uniforms[i]);
	}

	Uniform* Material::GetUniform(const char* name) const {
		if(uniforms.size() <= 0)
			return nullptr;

		auto index = indexing.find(name);
		assert(index != indexing.end());

		return uniforms[(*index).second];
	}
}