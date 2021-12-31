#pragma once
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

namespace BE {
	class Shader;
	class UniformBuffer {
	public:

		static void CreateUniformBufferFromShader(const unsigned int& shaderProgram);
		static UniformBuffer* GetUniformBuffer(const char* name);

		void SetValue(const int& value);
		void SetValue(const float& value);
		void SetValue(const glm::mat4& value);

	private:
		void BindUBO(const unsigned int& shaderProgram);

		UniformBuffer(const char* name, const unsigned int& size);
		~UniformBuffer() { /* TODO */ }

		std::string bufferName;
		unsigned int ubo;
		unsigned int bindingIndex; 

		static unsigned int bindingCount;
		static std::unordered_map<std::string, UniformBuffer*> buffers;
	};
}

