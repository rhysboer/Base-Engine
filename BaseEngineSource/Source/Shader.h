#pragma once
#include "glm/glm.hpp"
#include <string>

namespace BE {
	enum PremadeShaders
	{
		STANDARD,
		UNLIT_COLOUR,
		UNLIT_TEXTURE,
		UNLIT_VERTEX_COLOUR,
		FLAT,
		SCREEN,

		_ENGINE_SHADOW,
		_COUNT
	};

	class Uniform;
	class ShaderManager;
	class Shader {
		friend ShaderManager;
	public:
		
		~Shader();

		static Shader* CreateShader(const char* name, const char* vertexSource = nullptr, const char* fragmentSource = nullptr, const char* geometrySource = nullptr);
		static Shader* CreateShaderFromFile(const char* name, const std::string& path, bool useGeometry = false);
		static Shader* CreateComputeShader(const char* name, const char* computeSource = nullptr); // TODO

		void UseProgram() const;
		inline int ProgramID() const { return shaderProgram; }

		void SetFloat(const char* name, const float& value) const;
		void SetFloatArray(const char* name, const unsigned int& size, const float& value)const;
		void SetInt(const char* name, const int& value) const;
		void SetIntArray(const char* name, const unsigned int& size, const int& value) const;
		void SetMatrix3(const char* name, const glm::mat3& value) const;
		void SetMatrix4(const char* name, const glm::mat4& value) const;
		void SetVector2(const char* name, const glm::vec2& value) const;
		void SetVector2Array(const char* name, const unsigned int& size, const glm::vec2& value) const;
		void SetVector3(const char* name, const glm::vec3& value) const;
		void SetVector4(const char* name, const glm::vec4& value) const;
		void SetTextureUnit(const char* name, const unsigned int& textureUnit) const;
		void SetUniform(const Uniform& uniform) const;

		const std::string& GetName() const { return name; }

	protected:

		bool LoadShader(const char* vertSource, const char* fragSource, const char* geomShader = nullptr); // Graphics Shader
		bool LoadShader(const char* computeSource); // Compute Shader

	private:
		Shader(Shader*) = delete;
		Shader(const Shader&) = delete;
		Shader(const char* name, const char* vertexSource = nullptr, const char* fragmentSource = nullptr, const char* geometrySource = nullptr);

		bool ErrorHandler(unsigned int toTest, unsigned int statusType);

		std::string name;

		int shaderProgram;
	};

}