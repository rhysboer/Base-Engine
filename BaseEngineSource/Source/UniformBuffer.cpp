#include "UniformBuffer.h"
#include <glad/glad.h>
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"

#include "Logging.h"

namespace BE {
	unsigned int UniformBuffer::bindingCount = 0;
	std::unordered_map<std::string, UniformBuffer*> UniformBuffer::buffers = std::unordered_map<std::string, UniformBuffer*>();

	UniformBuffer::UniformBuffer(const char* name, const unsigned int& size) : bufferName(name), bindingIndex(bindingCount) {
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		++bindingCount;
	}

	void UniformBuffer::CreateUniformBufferFromShader(const unsigned int& shaderProgram) {
		int bufferCount = 0;
		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &bufferCount);

		int length = 0, bytes = 0;
		for(int i = 0; i < bufferCount; i++) {
			char name[100];
			glGetActiveUniformBlockName(shaderProgram, i, 100, &length, name);
			glGetActiveUniformBlockiv(shaderProgram, i, GL_UNIFORM_BLOCK_DATA_SIZE, &bytes);
			name[length] = 0;

			auto buffer = buffers.find(name);
			UniformBuffer* uniformBuffer = nullptr;
			if(buffer == buffers.end()) {
				uniformBuffer = new UniformBuffer(name, bytes);
				uniformBuffer->GenerateUniforms(shaderProgram, i);

				buffers.emplace(name, uniformBuffer);
			} else {
				uniformBuffer = (*buffer).second;
			}
				
			uniformBuffer->BindUBO(shaderProgram);
		}
	}

	void UniformBuffer::BindUBO(const unsigned int& shaderProgram) {
		unsigned int blockIndex = glGetUniformBlockIndex(shaderProgram, bufferName.c_str());

		// TODO:
		// bindingIndex should correspond to a unique bindingIndex for a shader sharing the same block
		// So if we have Camera in 2 different shaders, both binding index should be 0, since they're both the same
		// Update: The above TODO shouldn't need doing anymore, with how ive set it up

		glUniformBlockBinding(shaderProgram, blockIndex, bindingIndex);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, ubo);
	}

	void UniformBuffer::GenerateUniforms(const unsigned int& shaderProg, const unsigned int& blockIndex)
	{
		int size = 0;
		glGetActiveUniformBlockiv(shaderProg, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &size);
		unsigned int* indices = new unsigned int[size];
		glGetActiveUniformBlockiv(shaderProg, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (int*)indices);

		int length;
		for (int i = 0; i < size; i++) {
			Uniform* uniform = Uniform::CreateUniform(shaderProg, indices[i], false);
			if (uniform == nullptr)
				continue;

			uniforms.push_back(uniform);
		}

		std::sort(uniforms.begin(), uniforms.end(), [](const auto& a, const auto& b) {
			return a->GetOffset() < b->GetOffset();
		});
	}

	void UniformBuffer::SetValue(const unsigned int& index, const void* value, UniformType type)
	{
		BE_ASSERT(index < uniforms.size(), "UniformBuffer::SetValue - Index is above Uniform size");
		BE_ASSERT(value != nullptr, "UniformBuffer::SetValue - Value is a nullptr");

		Uniform* uniform = uniforms[index];

		BE_ASSERT(uniform->GetType() == type, "UniformBuffer::SetValue - Value type (%i) does not match Uniform type (%i)", type, uniform->GetType());

		glBindBuffer(GL_UNIFORM_BUFFER, ubo);

		// TODO: Remove the magic numbers
		switch (uniform->GetType())
		{
			case BE::UniformType::FLOAT: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 4, &value); break;
			case BE::UniformType::TEXTURE_2D:
			case BE::UniformType::INT:glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 4, &value); break;
			case BE::UniformType::VECTOR_2: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 8, value); break;
			case BE::UniformType::VECTOR_3: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 12, value); break;
			case BE::UniformType::VECTOR_4: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 16, value); break;
			case BE::UniformType::MATRIX_2: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 32, value); break;
			case BE::UniformType::MATRIX_3: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 48, value); break;
			case BE::UniformType::MATRIX_4: glBufferSubData(GL_UNIFORM_BUFFER, uniform->GetOffset(), 64, value); break;
			default: BE_ERROR("UniformBuffer::SetValue - Unsupported uniform type %i", uniform->GetType());
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	int UniformBuffer::FindIndex(const char* name) const
	{
		for (int i = 0; i < uniforms.size(); i++) {
			if (strcmp(uniforms[i]->GetName(), name) == 0)
				return i;
		}

		return -1;
	}

	UniformBuffer* UniformBuffer::GetUniformBuffer(const char* name) {
		auto buffer = buffers.find(name);
		return buffer != buffers.end() ? (*buffer).second : nullptr;
	}
}