#pragma once
#include "glm/glm.hpp"

namespace BE {
	class Uniform;
	class ShaderManager;
	class Shader {
		friend ShaderManager;
	public:
		Shader();
		~Shader();

		void UseProgram() const;
		inline int ProgramID() const { return shaderProgram; }

		void SetFloat(const char* name, const float& value) const;
		void SetFloatArray(const char* name, const unsigned int& size, const float& value)const;
		void SetInt(const char* name, const int& value) const;
		void SetIntArray(const char* name, const unsigned int& size, const int& value) const;
		void SetMatrix4(const char* name, const glm::mat4& value) const;
		void SetMatrix3(const char* name, const glm::mat3& value) const;
		void SetVector2(const char* name, const glm::vec2& value) const;
		void SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value) const;
		void SetVector3(const char* name, const glm::vec3& value) const;
		void SetVector4(const char* name, const glm::vec4& value) const;
		void SetTextureUnit(const char* name, const unsigned int& textureUnit) const;
		void SetUniform(const Uniform& uniform) const;

	protected:

		bool LoadShader(const char* vertSource, const char* fragSource, const char* geomShader = nullptr); // Graphics Shader
		bool LoadShader(const char* computeSource); // Compute Shader

	private:

		bool ErrorHandler(unsigned int toTest, unsigned int statusType);
		
		int shaderProgram;
	};

}