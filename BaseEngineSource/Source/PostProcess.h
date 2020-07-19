#pragma once
#include "Framebuffer.h"
#include "ShaderManager.h"

namespace BE {

	class PostProcess {
	public:

		PostProcess(Shader* shader, const float& sizeX, const float& sizeY, const FramebufferType& type, const unsigned int& colourAttachments = 1);
		~PostProcess();

		Shader& GetShader() const;
		Framebuffer& GetBuffer() const;

		void Begin();
		void End();

		void Render();

	private:

		Framebuffer* framebuffer;
		Shader* shader;

		unsigned int vao;
		unsigned int vbo;
	};
}

