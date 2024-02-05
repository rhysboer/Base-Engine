#include "Framebuffer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "BaseEngine.h"
#include "BEGlobal.h"
#include "EventSystem.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Logging.h"

/*
	Use renderbuffer for depth if you dont need to sample it later in a shader, but otherwise use a texture for depth so we can use it for sampling
*/


namespace BE {
	Framebuffer* Framebuffer::previousFramebuffer = nullptr;
	Framebuffer* Framebuffer::currentActiveFramebuffer = nullptr;

	Framebuffer::Framebuffer(const FramebufferType& type, const int& width, const int& height, const TextureDesc& textureDesc)
		: Framebuffer(type, { width, height }, textureDesc) {}

	// TODO: Chane size to Ivec2
	Framebuffer::Framebuffer(const FramebufferType& type, const glm::ivec2& size, const TextureDesc& textureDesc)
		: type(type), framebufferSize(size), /*prevViewportSize(0, 0),*/ isDirty(false), textureDesc(textureDesc)
	{

		if (size.x <= 0 || size.y <= 0) {
			//int x, y;
			BE::BaseEngine::GetWindowSize(framebufferSize.x, framebufferSize.y);
			useWindowSize = true;
			//framebufferSize = glm::vec2(x, y);

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
		//int x, y;
		//BaseEngine::GetWindowSize(x, y);
		//this->prevViewportSize = glm::vec2(x, y);
		//this->previousFramebuffer = currentActiveFramebuffer;

		// Should only ever get dirty if framebuffer size isn't fixed
		if (isDirty) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			glDeleteRenderbuffers(1, &rbo);
			glDeleteFramebuffers(1, &fbo);
			
			if (useWindowSize) {
				BaseEngine::GetWindowSize(framebufferSize.x, framebufferSize.y);
			}

			//framebufferSize = glm::vec2(x, y);
			isDirty = false;
			
			CreateFramebuffer(1);
		}
	
		glViewport(0, 0, framebufferSize.x, framebufferSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
		previousFramebuffer = currentActiveFramebuffer;
		currentActiveFramebuffer = this;

		ClearBuffer(true, true);
	}
	
	void Framebuffer::Render_End(const bool& enablePreviousBuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//ClearBuffer(true, true);
		//glViewport(0, 0, this->prevViewportSize.x, this->prevViewportSize.y);

		if (enablePreviousBuffer && previousFramebuffer != nullptr) {
			previousFramebuffer->Render_Begin();
		}
		else {
			currentActiveFramebuffer = nullptr;

			int x, y;
			BaseEngine::GetWindowSize(x, y);
			glViewport(0, 0, x, y);
		}
	}

	void Framebuffer::SetClearColour(const float& r, const float& g, const float& b, const float& a)
	{
		clearColour[0] = r;
		clearColour[1] = g;
		clearColour[2] = b;
		clearColour[3] = a;
	}

	void Framebuffer::ClearBuffer(const bool& colour, const bool& depth, const bool& stencil)
	{
		if (currentActiveFramebuffer != this) {
			BE_WARNING("Cannot clear buffer that isn't set as the active buffer");
			return;
		}

		glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]);
		unsigned int mask = (colour ? GL_COLOR_BUFFER_BIT : GL_ZERO) | (depth ? GL_DEPTH_BUFFER_BIT : GL_ZERO) | (stencil ? GL_STENCIL_BUFFER_BIT : GL_ZERO);
		glClear(mask);
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
		const bool bindFrameBuffer = currentActiveFramebuffer != this && currentActiveFramebuffer != nullptr;

		// [TODO] : Fix this so it can run without having the the framebuffer attach prier
		//if (bindFrameBuffer) {
		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//}

		// Set output texture as framebuffers texture
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

		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		Renderer::RenderQuad();

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Revert back to previous texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0]->GetID(), 0);
		

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Bind previous framebuffer
		//if (bindFrameBuffer) {
		//	glBindFramebuffer(GL_FRAMEBUFFER, currentActiveFramebuffer->fbo);
		//	glViewport(0, 0, currentActiveFramebuffer->framebufferSize.x, currentActiveFramebuffer->framebufferSize.y);
		//}
		//else {
			int x, y;
			BaseEngine::GetWindowSize(x, y);
			glViewport(0, 0, x, y);
		//}
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
		if (framebufferSize.x <= 0)
			framebufferSize.x = 1;
		if (framebufferSize.y <= 0)
			framebufferSize.y = 1;

		// Generate & bind Framebuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Create Colour Texture
		if(type != FramebufferType::DEPTH_TEX) {
	
			unsigned int texture;
			std::vector<unsigned int> attachments = std::vector<unsigned int>();

			if (textures.size() <= 0) {
				for (int i = 0; i < colourAttachments; i++) {

					// TODO: Dont hard set this, have textureDescs for both Colour & Depth
					//textureDesc.format = TextureFormat::RGB;

					Texture* texture = new Texture(framebufferSize.x, framebufferSize.y, textureDesc);

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
			// - Make sure to textureDesc is always set to Depth
			textureDesc.format = TextureFormat::DEPTH;

			if (!depthTexture) {
				depthTexture = new Texture(framebufferSize.x, framebufferSize.y, textureDesc);
			} else {
				depthTexture->Redefine(framebufferSize.x, framebufferSize.y, textureDesc);
			}
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetID(), 0);
			if (type == FramebufferType::DEPTH_TEX) {
				// Discard colour
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}
		}
	
		// Check if framebuffer is complete
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR: Framebuffer - failed to create framebuffer!");
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
