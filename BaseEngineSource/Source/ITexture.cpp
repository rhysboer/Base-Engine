#include "ITexture.h"

namespace BE {
	int ITexture::nextActiveTexture = 0;

	ITexture::ITexture(unsigned int width, unsigned int height) : sizeX(width), sizeY(height)
	{
	}

	ITexture::~ITexture() = default;

	void ITexture::Bind() const
	{
		if (textureId == 0)
			return;

		if (activeIndex == -1)
			activeIndex = CreateActiveID();

		glActiveTexture(GL_TEXTURE0 + activeIndex); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	unsigned int ITexture::GetActiveID() const
	{
		if (activeIndex == -1)
			activeIndex = CreateActiveID();
		return activeIndex;
	}

	void ITexture::DestroyTexture()
	{
		glDeleteTextures(1, &textureId);
	}

	void ITexture::CreateTextureObject()
	{
		// TODO: Change 1 to be a parameter
		glGenTextures(1, &textureId);
	}

	void ITexture::SetPixels(const std::vector<unsigned char>& pixels)
	{
		if (textureId == 0)
			return;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, (GLint)format, GL_UNSIGNED_BYTE, &pixels[0]);
	}
}

