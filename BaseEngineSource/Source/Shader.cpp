#include "Shader.h"
#include "ShaderManager.h"
#include "File.h"
#include "BEMath.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Uniform.h"
#include "ITexture.h"
#include "Logging.h"
#include "glm/gtc/type_ptr.hpp"

#define FRAG_APPEND ".frag"
#define VERT_APPEND ".vert"
#define GEOM_APPEND ".geom"

namespace BE {
	Shader::Shader(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource)
		: shaderProgram(-1), name(name)
	{
		LoadShader(vertexSource, fragmentSource, geometrySource);
		BE::ShaderManager::RegisterShader(this);
	}

	Shader* Shader::CreateShader(const char* name, const char* vertexSource, const char* fragmentSource, const char* geometrySource)
	{
		Shader* shader = nullptr;

		std::string shaderName = name;
		std::string vertex = vertexSource ? vertexSource : "";
		std::string fragment = fragmentSource ? fragmentSource : "";
		std::string geometry = geometrySource ? geometrySource : "";

		if (vertexSource == nullptr || vertex.empty() || fragmentSource == nullptr || fragment.empty()) {
			BE_WARNING("Shader '%s', has no valid vertex or fragment shader", name);
			return nullptr;
		}

		BE::ShaderManager::TryAddHeaders(vertex);
		BE::ShaderManager::TryAddHeaders(fragment);
		BE::ShaderManager::TryAddHeaders(geometry);

		if (BE::ShaderManager::GetShader(name) != nullptr) {
			BE_WARNING("Shader with name '%s' already exists!", name);
			shaderName = name + std::to_string(BE::Math::RandomInt());
		}

		shader = new Shader(shaderName.c_str(), vertex.c_str(), fragment.c_str(), geometry.empty() ? nullptr : geometry.c_str());
		return shader;
	}

	Shader* Shader::CreateShaderFromFile(const char* name, const std::string& path, bool useGeometry)
	{
		std::string vertex = File::LoadFile(path + VERT_APPEND);
		std::string fragment = File::LoadFile(path + FRAG_APPEND);
		std::string geometry = useGeometry ? File::LoadFile(path + GEOM_APPEND) : "";

		return CreateShader(name, vertex.c_str(), fragment.c_str(), useGeometry ? geometry.c_str() : nullptr);
	}

	Shader::~Shader() {
		if(shaderProgram != 0)
			glDeleteProgram(shaderProgram);

		BE::ShaderManager::DeregisterShader(this);
	}

	bool Shader::LoadShader(const char* vertSource, const char* fragSource, const char* geomShader) {
		unsigned int shaderVertex;
		unsigned int shaderFragment;
		unsigned int shaderGeometry;

		shaderVertex = glCreateShader(GL_VERTEX_SHADER);
		shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
		if(geomShader != nullptr)
			shaderGeometry = glCreateShader(GL_GEOMETRY_SHADER);

		// VERTEX
		glShaderSource(shaderVertex, 1, &vertSource, NULL);
		glCompileShader(shaderVertex);

		if(!ErrorHandler(shaderVertex, GL_COMPILE_STATUS)) 
			return false;

		// FRAGMENT
		glShaderSource(shaderFragment, 1, &fragSource, NULL);
		glCompileShader(shaderFragment);

		if(!ErrorHandler(shaderFragment, GL_COMPILE_STATUS)) 
			return false;

		// GEOMETRY
		if(geomShader != nullptr) {
			glShaderSource(shaderGeometry, 1, &geomShader, NULL);
			glCompileShader(shaderGeometry);

			if(!ErrorHandler(shaderGeometry, GL_COMPILE_STATUS)) 
				return false;
		}

		// LINK SHADER
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shaderVertex);
		glAttachShader(shaderProgram, shaderFragment);
		if(geomShader != nullptr)
			glAttachShader(shaderProgram, shaderGeometry);

		glLinkProgram(shaderProgram);

		if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) 
			return false;

		// Delete
		glDeleteShader(shaderVertex);
		glDeleteShader(shaderFragment);
		if(geomShader != nullptr)
			glDeleteShader(shaderGeometry);

		return true;
	}

	bool Shader::LoadShader(const char* computeSource) {
		unsigned int shaderIndex;

		shaderIndex = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shaderIndex, 1, &computeSource, NULL);
		glCompileShader(shaderIndex);

		if(!ErrorHandler(shaderIndex, GL_COMPILE_STATUS)) 
			return false;

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shaderIndex);
		glLinkProgram(shaderProgram);

		if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) 
			return false;

		glDeleteShader(shaderIndex);

		return true;
	}

	void Shader::UseProgram() const {
		glUseProgram(shaderProgram);
	}

	void Shader::SetFloat(const char* name, const float& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetFloat() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniform1f(loc, value);
	}

	void Shader::SetFloatArray(const char* name, const unsigned int& size, const float& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetFloatArray() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}

		glUniform1fv(loc, size, &value);
	}

	void Shader::SetInt(const char* name, const int& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetInt() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniform1i(loc, value);
	}

	void Shader::SetIntArray(const char* name, const unsigned int& size, const int& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetIntArray() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniform1iv(loc, size, &value);
	}

	void Shader::SetMatrix4(const char* name, const glm::mat4& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetMatrix4() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMatrix3(const char* name, const glm::mat3& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::Setmatrix3() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetVector2(const char* name, const glm::vec2& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetVector2() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}

		glUniform2f(loc, value.r, value.g);
	}

	void Shader::SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetVector2Array() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}

		glUniform2fv(loc, size, &value[0]);
	}

	void Shader::SetVector4(const char* name, const glm::vec4& value)const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetVector4() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}

		glUniform4f(loc, value.r, value.g, value.b, value.a);
	}

	void Shader::SetVector3(const char* name, const glm::vec3& value)const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetVector3() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}

		glUniform3f(loc, value.r, value.g, value.b);
	}

	void Shader::SetTextureUnit(const char* name, const unsigned int& textureUnit) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			BE_ERROR("Shader::SetTextureUnit() - Failed to find location of Uniform (Name: %s)", name);
			return;
		}
		glUniform1i(loc, textureUnit);
	}

	void Shader::SetUniform(const Uniform& uniform) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, uniform.GetName());
		BE_ASSERT(loc != -1, "Shader::SetUniform() - Failed to find location of Uniform (Name: %s, Type: %i)", uniform.GetName(), uniform.GetType());

		switch(uniform.GetType()) {
		case UniformType::TEXTURE_2D:
		{
			ITexture* texture = (ITexture*)uniform.GetValue(0);
			texture->Bind();
			glUniform1i(loc, texture->GetActiveID());
			break;
		}
		case UniformType::BOOLEAN: 
		case UniformType::INT: glUniform1i(loc, (int)uniform.GetValue(0)); break;
		case UniformType::FLOAT: glUniform1f(loc, *(float*)uniform.GetValue(0)); break;
		case UniformType::VECTOR_2: glUniform2f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1)); break;
		case UniformType::VECTOR_3: glUniform3f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1), *(float*)uniform.GetValue(2)); break;
		case UniformType::VECTOR_4: glUniform4f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1), *(float*)uniform.GetValue(2), *(float*)uniform.GetValue(3)); break;;
		case UniformType::MATRIX_3: break; // TODO
		case UniformType::MATRIX_4: glUniformMatrix4fv(loc, 1, false, (float*)uniform.GetValue()); break;
		default: BE_ERROR("Shader::SetUniform() - Unsupported uniform type (%i)", uniform.GetType());
		}
	}

	bool Shader::ErrorHandler(unsigned int toTest, unsigned int statusType) {
		int success;
		int logSize = 0;

		char* log = nullptr;

		switch(statusType) {
			case GL_COMPILE_STATUS: // SHADER
				glGetShaderiv(toTest, GL_INFO_LOG_LENGTH, &logSize);
				glGetShaderiv(toTest, GL_COMPILE_STATUS, &success);

				log = new char[logSize];

				if(!success) {
					glGetShaderInfoLog(toTest, logSize, NULL, log);
					BE_ERROR("Shader Error: '%s' failed to compile shader (Error: %s)", name.c_str(), log);
				}

				break;
			case GL_LINK_STATUS: // LINK
				glGetProgramiv(toTest, GL_INFO_LOG_LENGTH, &logSize);
				glGetProgramiv(toTest, GL_LINK_STATUS, &success);

				log = new char[logSize];

				if(!success) {
					glGetProgramInfoLog(toTest, logSize, NULL, log);
					BE_ERROR("Shader Error: '%s' failed to link shader (Error: %s)", name.c_str(), log);
				}

				break;
		}

		if(log != nullptr)
			delete[] log;

		return true;
	}
}