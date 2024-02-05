#pragma once
#include <vector>
#include "glm/vec2.hpp"
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
		Framebuffer(const FramebufferType& type, const int& width, const int& height, const TextureDesc& textureDesc = {});
		Framebuffer(const FramebufferType& type, const glm::ivec2& size = { 0, 0 }, const TextureDesc& textureDesc = {});
		~Framebuffer();

		void Render_Begin();
		void Render_End(const bool& enablePreviousBuffer = false);
		void Bind();

		void SetClearColour(const float& r, const float& g, const float& b, const float& a);
		void ClearBuffer(const bool& colour = false, const bool& depth = false, const bool& stencil = false);


		// TODO: Implement
		// SetSize(const glm::vec2& size);

		void CopyColorTo(Texture* const texture, Material* const material = nullptr) const;

		void BindTextureColour() const;
		void BindTextureDepth(const unsigned int& index) const;

		Texture* const GetTexture(unsigned int index) const { return textures[0]; }
		Texture* GetDepthTexture() { return depthTexture; }

	private:
		static Framebuffer* previousFramebuffer;
		static Framebuffer* currentActiveFramebuffer;

		void CreateFramebuffer(const unsigned int& colourAttachments = 1);

		glm::ivec2 framebufferSize;
		//glm::vec2 prevViewportSize; // Viewport size before resizing
		
		FramebufferType type;
		
		unsigned int fbo; // Framebuffer
		unsigned int rbo; // Renderbuffer
		
		TextureDesc textureDesc = {};

		std::vector<Texture*> textures = std::vector<Texture*>();
		Texture* depthTexture = nullptr;

		float clearColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		unsigned int eventId;
		bool useWindowSize = false;
		bool isDirty = false;
	};
}

