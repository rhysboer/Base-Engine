#include "UniformBuffer.h"
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

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
			glGetActiveUniformBlockName(shaderProgram, i, sizeof(name - 1), &length, name);
			glGetActiveUniformBlockiv(shaderProgram, i, GL_UNIFORM_BLOCK_DATA_SIZE, &bytes);
			name[length] = 0;

			auto buffer = buffers.find(name);
			UniformBuffer* uniformBuffer = nullptr;
			if(buffer == buffers.end()) {
				uniformBuffer = new UniformBuffer(name, bytes);
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

	void UniformBuffer::SetValue(const glm::mat4& value) {
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(value));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	UniformBuffer* UniformBuffer::GetUniformBuffer(const char* name) {
		auto buffer = buffers.find(name);
		return buffer != buffers.end() ? (*buffer).second : nullptr;
	}
}