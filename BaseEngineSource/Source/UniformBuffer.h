#pragma once
#include <unordered_map>
#include <string>
#include "Uniform.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace BE {
	class Shader;
	class UniformBuffer {
	public:

		static void CreateUniformBufferFromShader(const unsigned int& shaderProgram);
		static UniformBuffer* GetUniformBuffer(const char* name);

		inline void SetValue(const unsigned int& index, const int& value) { SetValue(index, (const void*&)value, UniformType::INT); }
		inline void SetValue(const unsigned int& index, const unsigned int& value) { SetValue(index, (const void*&)value, UniformType::INT); }
		inline void SetValue(const unsigned int& index, const float& value) { SetValue(index, (const void*&)value, UniformType::FLOAT); }
		inline void SetValue(const unsigned int& index, const glm::vec2& value) { SetValue(index, (const void*&)value, UniformType::VECTOR_2); }
		inline void SetValue(const unsigned int& index, const glm::vec3& value) { SetValue(index, (const void*&)value, UniformType::VECTOR_3); }
		inline void SetValue(const unsigned int& index, const glm::vec4& value) { SetValue(index, (void*)glm::value_ptr(value), UniformType::VECTOR_4); }
		inline void SetValue(const unsigned int& index, const glm::mat2& value) { SetValue(index, (const void*&)value, UniformType::MATRIX_2); }
		inline void SetValue(const unsigned int& index, const glm::mat3& value) { SetValue(index, (const void*&)value, UniformType::MATRIX_3); }
		inline void SetValue(const unsigned int& index, const glm::mat4& value) { SetValue(index, (void*)glm::value_ptr(value), UniformType::MATRIX_4); }

		int FindIndex(const char* name) const;

	private:

		void BindUBO(const unsigned int& shaderProgram);
		void GenerateUniforms(const unsigned int& shaderProg, const unsigned int& blockIndex);
		void SetValue(const unsigned int& index, const void* value, UniformType type);

		UniformBuffer(const char* name, const unsigned int& size);
		~UniformBuffer() { /* TODO */ }

		std::string bufferName;
		unsigned int ubo;
		unsigned int bindingIndex; 

		std::vector<Uniform*> uniforms{};

		static unsigned int bindingCount;
		static std::unordered_map<std::string, UniformBuffer*> buffers;
	};
}

