#include "Framebuffer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "BaseEngine.h"
#include "BEGlobal.h"
#include "EventSystem.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Material.h"

/*
	Use renderbuffer for depth if you dont need to sample it later in a shader, but otherwise use a texture for depth so we can use it for sampling
*/


namespace BE {
	Framebuffer::Framebuffer(const FramebufferType& type, const glm::vec2& size)
		: type(type), framebufferSize(size), prevViewportSize(0), isDirty(false)
	{
		// TODO: Chane size to Ivec2
		
		if (size.x <= 0 || size.y <= 0) {
			int x, y;
			BE::BaseEngine::GetWindowSize(x, y);
			framebufferSize = glm::vec2(x, y);
			resize = true;

			eventId = BE::EventSystem::EventSubscribe(BE_EVENT_WINDOW_RESIZE, [&](const void* const) { isDirty = true; });
		}

		CreateFramebuffer(1);
	}
	
	Framebuffer::~Framebuffer() {
		// Delete Textures
		for(int i = 0; i < textures.size(); i++)
			delete textures[i];

		delete depthTexture;
		
		// Delete Buffers
		glDeleteRenderbuffers(1, &rbo);
		glDeleteFramebuffers(1, &fbo);

		if (eventId != 0) {
			BE::EventSystem::EventUnsubscribe(BE_EVENT_WINDOW_RESIZE, eventId);
		}
	}
	
	void Framebuffer::Render_Begin() {
		int x, y;
		BaseEngine::GetWindowSize(x, y);
		this->prevViewportSize = glm::vec2(x, y);

		// Should only ever get dirty if framebuffer size isn't fixed
		if (isDirty) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			glDeleteRenderbuffers(1, &rbo);
			glDeleteFramebuffers(1, &fbo);
			
			framebufferSize = glm::vec2(x, y);
			isDirty = false;
			
			CreateFramebuffer(1);
		}
	
		glViewport(0, 0, framebufferSize.x, framebufferSize.y);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
		Renderer::ClearBuffer(true, true);
	}
	
	void Framebuffer::Render_End() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Renderer::ClearBuffer(true, true);
		glViewport(0, 0, this->prevViewportSize.x, this->prevViewportSize.y);
	}
	
	//void Framebuffer::Bind() {
	//	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//}
	//
	//void Framebuffer::SetSize(const glm::vec2& size) {
	//	this->framebufferSize = size;
	//
	//	for(int i = 0; i < colourTexture.size(); i++) {
	//		glDeleteTextures(1, &colourTexture[i]);
	//	}
	//	glDeleteTextures(1, &depthTexture);
	//
	//	glDeleteFramebuffers(1, &fbo);
	//	glDeleteRenderbuffers(1, &rbo);
	//
	//	CreateFramebuffer(totalColourAttachments);
	//}
	
	void Framebuffer::CopyColorTo(Texture* const texture, Material* const material) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);

		glm::vec2 size = texture->GetSize();
		glViewport(0, 0, size.x, size.y);

		if (material != nullptr) {
			material->SetMainTexture(textures[0]);
			material->Use();
		} else {
			Renderer::ScreenQuadMaterial()->SetMainTexture(textures[0]);
			Renderer::ScreenQuadMaterial()->Use();
		}

		glDisable(GL_DEPTH_TEST);

		Renderer::RenderQuad();
	
		glEnable(GL_DEPTH_TEST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0]->GetID(), 0);
		glViewport(0, 0, prevViewportSize.x, prevViewportSize.y);
	}

	void Framebuffer::BindTextureColour() const {
		if(!textures.empty()) {
			textures[0]->Bind();
		}
	}
	
	//void Framebuffer::BindTextureDepth(const unsigned int& index) const {
	//	if(depthTexture != 0) {
	//		glActiveTexture(GL_TEXTURE0 + index);
	//		glBindTexture(GL_TEXTURE_2D, depthTexture);
	//	}
	//}
	//
	////unsigned int Framebuffer::GetTexture(unsigned int index) const {
	////	if(index >= colourTexture.size()) return -1;
	////	return colourTexture[index];
	////}

	void Framebuffer::CreateFramebuffer(const unsigned int& colourAttachments) {
	
		// Generate & bind Framebuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Create Colour Texture
		if(type != FramebufferType::DEPTH_TEX) {
	
			unsigned int texture;
			std::vector<unsigned int> attachments = std::vector<unsigned int>();

			if (textures.size() <= 0) {
				for (int i = 0; i < colourAttachments; i++) {
					Texture* texture = new Texture(framebufferSize.x, framebufferSize.y);

					// Attach Texture to Framebuffer
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->GetID(), 0);
					attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);

					textures.emplace_back(texture);
				}
			}
			else {
				// Reuse textures after resize
				for (int i = 0; i < textures.size(); i++) {
					textures[i]->Redefine(framebufferSize.x, framebufferSize.y);
				
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i]->GetID(), 0);
					attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
				}
			}
	
			glDrawBuffers(colourAttachments, attachments.data());
	
			// Create Depth Renderer
			if(type == FramebufferType::COLOUR_TEX_DEPTH) {
				// Generate Render Buffer (for depth & stencil)
				glGenRenderbuffers(1, &rbo);
	
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize.x, framebufferSize.y);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
		}
	
		// Create Depth Texture
		if(type == FramebufferType::DEPTH_TEX || type == FramebufferType::COLOUR_TEX_DEPTH_TEX) {
			TextureDesc desc = TextureDesc();
			desc.format = TextureFormat::DEPTH;

			if (!depthTexture) {
				depthTexture = new Texture(framebufferSize.x, framebufferSize.y, desc);
			} else {
				depthTexture->Redefine(framebufferSize.x, framebufferSize.y, desc);
			}
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetID(), 0);
		}
	
		// Check if framebuffer is complete
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: Framebuffer - failed to create framebuffer!");
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
