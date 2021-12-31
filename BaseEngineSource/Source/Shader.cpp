#include "Shader.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Uniform.h"
#include "glm/gtc/type_ptr.hpp"

namespace BE {
	Shader::Shader() : 
		shaderProgram(-1) 
	{}

	Shader::~Shader() {
		if(shaderProgram != 0)
			glDeleteProgram(shaderProgram);
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

		if(!ErrorHandler(shaderVertex, GL_COMPILE_STATUS)) return false;

		// FRAGMENT
		glShaderSource(shaderFragment, 1, &fragSource, NULL);
		glCompileShader(shaderFragment);

		if(!ErrorHandler(shaderFragment, GL_COMPILE_STATUS)) return false;

		// GEOMETRY
		if(geomShader != nullptr) {
			glShaderSource(shaderGeometry, 1, &geomShader, NULL);
			glCompileShader(shaderGeometry);

			if(!ErrorHandler(shaderGeometry, GL_COMPILE_STATUS)) return false;
		}

		// LINK SHADER
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shaderVertex);
		glAttachShader(shaderProgram, shaderFragment);
		if(geomShader != nullptr)
			glAttachShader(shaderProgram, shaderGeometry);

		glLinkProgram(shaderProgram);

		if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) return false;

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

		if(!ErrorHandler(shaderIndex, GL_COMPILE_STATUS)) return false;

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shaderIndex);
		glLinkProgram(shaderProgram);

		if(!ErrorHandler(shaderProgram, GL_LINK_STATUS)) return false;

		glDeleteShader(shaderIndex);

		return true;
	}


	void Shader::UseProgram() const {
		glUseProgram(shaderProgram);
	}

	void Shader::SetFloat(const char* name, const float& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding float Uniform: %s\n", name);
			return;
		}
		glUniform1f(loc, value);
	}

	void Shader::SetFloatArray(const char* name, const unsigned int& size, const float& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding float Uniform: %s\n", name);
			return;
		}

		glUniform1fv(loc, size, &value);
	}

	void Shader::SetInt(const char* name, const int& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding int Uniform: %s\n", name);
			return;
		}
		glUniform1i(loc, value);
	}

	void Shader::SetIntArray(const char* name, const unsigned int& size, const int& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding int Uniform: %s\n", name);
			return;
		}
		glUniform1iv(loc, size, &value);
	}

	void Shader::SetMatrix4(const char* name, const glm::mat4& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Mat4 Uniform: %s\n", name);
			return;
		}
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMatrix3(const char* name, const glm::mat3& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Mat4 Uniform: %s\n", name);
			return;
		}
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetVector2(const char* name, const glm::vec2& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Vec4 Uniform: %s\n", name);
			return;
		}

		glUniform2f(loc, value.r, value.g);
	}

	void Shader::SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Vec3 Uniform: %s\n", name);
			return;
		}

		glUniform2fv(loc, size, &value[0]);
	}

	void Shader::SetVector4(const char* name, const glm::vec4& value)const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Vec4 Uniform: %s\n", name);
			return;
		}

		glUniform4f(loc, value.r, value.g, value.b, value.a);
	}

	void Shader::SetVector3(const char* name, const glm::vec3& value)const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Vec3 Uniform: %s\n", name);
			return;
		}

		glUniform3f(loc, value.r, value.g, value.b);
	}

	void Shader::SetTextureUnit(const char* name, const unsigned int& textureUnit) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, name);
		if(loc == -1) {
			printf("Error finding Mat4 Uniform: %s\n", name);
			return;
		}
		glUniform1i(loc, textureUnit);
	}

	void Shader::SetUniform(const Uniform& uniform) const {
		unsigned int loc = glGetUniformLocation(shaderProgram, uniform.GetName());
		assert(loc != -1);

		//float ass = *(float*)uniform.GetValue(2);

		switch(uniform.GetType()) {
			case UniformType::INT: break;
			case UniformType::FLOAT: glUniform1f(loc, *(float*)uniform.GetValue(0)); break;
			case UniformType::VECTOR_2: glUniform2f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1)); break;
			case UniformType::VECTOR_3: glUniform3f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1), *(float*)uniform.GetValue(2)); break;
			case UniformType::VECTOR_4: glUniform4f(loc, *(float*)uniform.GetValue(0), *(float*)uniform.GetValue(1), *(float*)uniform.GetValue(2), *(float*)uniform.GetValue(3)); break;;
			case UniformType::MATRIX_3: break;
			case UniformType::MATRIX_4: glUniformMatrix4fv(loc, 1, false, (float*)uniform.GetValue()); break;
		}
	}

	bool Shader::ErrorHandler(unsigned int toTest, unsigned int statusType) {
		int success;
		int logSize = 0;

		glGetShaderiv(toTest, GL_INFO_LOG_LENGTH, &logSize);
		char* log = new char[logSize];

		switch(statusType) {
			case GL_COMPILE_STATUS: // SHADER
				glGetShaderiv(toTest, GL_COMPILE_STATUS, &success);

				if(!success) {
					glGetShaderInfoLog(toTest, logSize, NULL, log);
					std::cout << "ERROR COMPILING SHADER FAILED!\n" << log << std::endl;
					return false;
				}

				break;
			case GL_LINK_STATUS: // LINK
				glGetProgramiv(toTest, GL_LINK_STATUS, &success);

				if(!success) {
					glGetProgramInfoLog(toTest, logSize, NULL, log);
					std::cout << "ERROR LINKING FAILED\n" << log << std::endl;
					return false;
				}

				break;
			default:
				printf("No Shader Error Status: %i\n", statusType);
				return false;
		}

		delete[] log;
		return true;
	}
}