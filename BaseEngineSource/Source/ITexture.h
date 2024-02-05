#pragma once
#include "glad/glad.h"
#include "glm/vec2.hpp"
#include <vector>


namespace BE {
	enum class TextureFlags
	{
		NONE,
		INVERT_R = 1 << 0,
		INVERT_G = 1 << 1,
		INVERT_B = 1 << 2,
		INVERT_A = 1 << 3
	};

	enum class TextureFormat
	{
		R = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

	enum class Wrapping
	{
		REPEAT = GL_REPEAT,
		MIRROR_REPEAT = GL_MIRRORED_REPEAT,
		MIRROR_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
		EDGE = GL_CLAMP_TO_EDGE,
		BORDER = GL_CLAMP_TO_BORDER
	};

	enum class Filtering {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	struct TextureDesc {
		TextureFormat format = TextureFormat::RGB;
		TextureFlags flags = TextureFlags::NONE;
		Wrapping wrapping = Wrapping::REPEAT;
		Filtering filter = Filtering::LINEAR;

		// - Wrapping has to be set to BORDER
		float borderColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	class ITexture
	{
	public:
		ITexture(unsigned int width, unsigned int height);
		virtual ~ITexture() = 0;

		void Bind() const;
		unsigned int GetActiveID() const;
		inline unsigned int GetID() const { return textureId; }
		inline glm::vec2 GetSize() const { return glm::vec2(sizeX, sizeY); }
		inline TextureFormat GetFormat() const { return format; }
		inline unsigned int GetChannels() const { return FormatToChannels(format); }

		virtual void SetPixels(const std::vector<unsigned char>& pixels);

		static inline int ChannelsToFormat(const int& channels);
		static inline int FormatToChannels(const TextureFormat& format);

	protected:
		
		void DestroyTexture();
		void CreateTextureObject();
		int GetFormat(const int& format) const;

		TextureFormat format;
		int sizeX, sizeY;

	private:
		
		unsigned int textureId = 0;
		
		mutable int activeIndex = -1;

		static inline int CreateActiveID() { nextActiveTexture++; return nextActiveTexture - 1; }
		static int nextActiveTexture;
	};

	inline int ITexture::ChannelsToFormat(const int& channels)
	{
		switch (channels) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}

		return GL_RGB;
	}

	inline int ITexture::FormatToChannels(const TextureFormat& format)
	{
		switch (format) {
		case TextureFormat::R: return 1;
		case TextureFormat::RG: return 2;
		case TextureFormat::RGB: return 3;
		case TextureFormat::RGBA: return 4;
		}

		return 3;
	}
}

