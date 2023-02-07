#pragma once
#include "glad/glad.h"

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

	//enum class TextureFilter {
	//	LINEAR,
	//
	//};

	struct TextureDesc {
		TextureFormat format = TextureFormat::RGB;
		TextureFlags flags = TextureFlags::NONE;
	};

	class ITexture
	{
	public:
		ITexture() {}
		virtual ~ITexture() = 0;

		void Bind() const;
		unsigned int GetActiveID() const;
		inline unsigned int GetID() const { return textureId; }


		static inline int ChannelsToFormat(const int& channels);
		static inline int FormatToChannels(const TextureFormat& format);

	protected:
		
		void DestroyTexture();
		void CreateTextureObject();
		int GetFormat(const int& format) const;

		TextureFormat format;

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

