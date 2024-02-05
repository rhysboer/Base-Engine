#include "Material.h"
#include "Uniform.h"
#include "ITexture.h"
#include "ShaderManager.h"
#include "UniformBuffer.h"
#include "Logging.h"
#include <glad/glad.h>

namespace BE {
	Material::Material(const char* shaderName) : Material(ShaderManager::GetShader(shaderName)) {}
	Material::Material(const PremadeShaders id) : Material(ShaderManager::GetPremadeShader(id)) {}
	Material::Material() : Material(ShaderManager::GetPremadeShader(PremadeShaders::STANDARD)) {}

	Material::Material(const Shader const * shader) : shader(shader) {
		int uniformCount = 0;
		
		glGetProgramiv(shader->ProgramID(), GL_ACTIVE_UNIFORMS, &uniformCount);

		for(int i = 0; i < uniformCount; i++) {
			Uniform* uniform = Uniform::CreateUniform(shader->ProgramID(), (unsigned int)i);
			if (uniform == nullptr)
				continue;

			indexing.emplace(uniform->GetName(), uniforms.size());
			uniforms.push_back(uniform);
		}

		UniformBuffer::CreateUniformBufferFromShader(shader->ProgramID());
	}

	Material::~Material() {
		// TODO: Delete Materials
	}

	void Material::SetUniform(const char* name, const glm::mat4& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find mat4 uniform with name of %s", name);
			return;
		}

		uniform->SetValue((void*)&value[0][0], 16);
	}

	void Material::SetUniform(const char* name, const glm::vec2& value, const unsigned int& size)
	{
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find vec2 uniform with name of %s", name);
			return;
		}

		uniform->SetValue((void*)&value[0], 2);
	}

	void Material::SetUniform(const char* name, const glm::vec3& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find vec3 uniform with name of %s", name);
			return;
		}

		uniform->SetValue((void*)&value[0], 3);
	}

	void Material::SetUniform(const char* name, const glm::vec4& value, const unsigned int& size)
	{
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find vec4 uniform with name of %s", name);
			return;
		}

		uniform->SetValue((void*)&value[0], 4);
	}

	void Material::SetUniform(const char* name, const int& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find int uniform with name of %s", name);
			return;
		}

		uniform->SetValue(&value, size);
	}

	void Material::SetUniform(const char* name, const float& value, const unsigned int& size) {
		Uniform* uniform = GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetUniform() - Failed to find float uniform with name of %s", name);
			return;
		}

		uniform->SetValue(&value, size);
	}

	void Material::SetUniform(const char* name, const  ITexture* texture)
	{
		UniformTexture* uniform = (UniformTexture*)GetUniform(name);
		if (uniform == nullptr) {
			BE_ERROR("Material::SetTexture() - Failed to find texture uniform with name of %s", name);
			return;
		}

		uniform->SetValue(texture);
	}

	void Material::SetMainTexture(const ITexture* texture)
	{
		SetUniform("mainTexture", texture);
	}

	ITexture* Material::GetMainTexture() const
	{
		UniformTexture* uniform = (UniformTexture*)GetUniform("mainTexture");
		if (uniform == nullptr) {
			BE_ERROR("Material::GetMainTexture() - Failed to find uniform with name of mainTexture");
			return nullptr;
		}
		
		return (ITexture*)uniform->GetValue(0);
	}

	void Material::SetTransparent(const bool& isTransparent)
	{
		this->isTransparent = isTransparent;
	}

	void Material::Use() const {
		shader->UseProgram();

		for(int i = 0; i < uniforms.size(); i++)
			shader->SetUniform(*uniforms[i]);

		// TODO: Change this so its not setting for every single object
		if (!depthWrite) {
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_ALWAYS);
		}
		else 
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}
	}

	Uniform* Material::GetUniform(const char* name) const {
		if(uniforms.size() <= 0)
			return nullptr;

		auto index = indexing.find(name);
		return index != indexing.end() ? uniforms[(*index).second] : nullptr;
	}

	Uniform* Material::GetUniform(const int& index) const
	{
		if (index < 0 || index >= uniforms.size())
			return nullptr;

		return uniforms[index];
	}
}