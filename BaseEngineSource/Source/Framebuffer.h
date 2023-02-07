#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Texture.h"

namespace BE {

	enum class FramebufferType {
		DEPTH_TEX, // Depth texture
		COLOUR_TEX, // Colour texture with NO renderbuffer
		COLOUR_TEX_DEPTH, // Colour texture with renderbuffer depth
		COLOUR_TEX_DEPTH_TEX, // Colour and depth texture
	};

	class Material;
	class Framebuffer {
	public:
		/// <param name="size">Set size to 0 to auto resize to screen size</param>
		Framebuffer(const FramebufferType& type, const glm::vec2& size = { 0, 0 });
		~Framebuffer();

		void Render_Begin();
		void Render_End();
		void Bind();

		// TODO: Implement
		// SetSize(const glm::vec2& size);

		void CopyColorTo(Texture* const texture, Material* const material = nullptr) const;

		void BindTextureColour() const;
		void BindTextureDepth(const unsigned int& index) const;

		Texture* const GetTexture(unsigned int index) const { return textures[0]; }

	private:

		void CreateFramebuffer(const unsigned int& colourAttachments = 1);

		glm::vec2 framebufferSize;
		glm::vec2 prevViewportSize; // Viewport size before resizing
		
		FramebufferType type;
		
		unsigned int fbo; // Framebuffer
		unsigned int rbo; // Renderbuffer
		
		std::vector<Texture*> textures = std::vector<Texture*>();
		Texture* depthTexture = nullptr;

		unsigned int eventId;
		bool resize = false;
		bool isDirty = false;
	};
}

