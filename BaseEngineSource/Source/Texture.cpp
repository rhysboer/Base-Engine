#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Texture.h"
#include "stb_image.h"
#include "Logging.h"
#include "BEMath.h"
#include <glad/glad.h>

#define ERROR_COL_ONE glm::vec3(255, 0, 255)
#define ERROR_COL_TWO glm::vec3(0, 0, 0)

namespace BE {
	Texture::Texture(const std::string& imageData, const TextureDesc& desc, bool overrideFormat) : ITexture(0, 0)
	{
		int channels;

		// Flip if opengl
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load_from_memory(
			(unsigned char*)imageData.c_str(),
			imageData.length(),
			&sizeX, &sizeY,
			&channels,
			overrideFormat ? FormatToChannels(desc.format) : 0
		);

		TextureDesc newDesc = desc;
		if (!overrideFormat)
		{
			newDesc.format = (TextureFormat)ChannelsToFormat(channels);
		}

		if (data == nullptr) {
			Logging::Error("Failed to load texture");
			data = GenerateColourTexture(64, 64, ERROR_COL_ONE, ERROR_COL_TWO);
			newDesc.format = TextureFormat::RGBA;
			newDesc.filter = Filtering::NEAREST;
		}

		CreateTexture(data, newDesc);
		stbi_image_free(data);
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const TextureDesc& desc) : ITexture(x, y)
	{
		auto pixels = GenerateColourTexture(x, y, glm::vec3(128, 128, 128), glm::vec3(255, 255, 255), ITexture::FormatToChannels(desc.format));
		CreateTexture(pixels, desc);

		delete[] pixels;
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const glm::vec3& colour, const TextureDesc& desc) : ITexture(x, y)
	{
		auto pixels = GenerateColourTexture(x, y, colour, colour, ITexture::FormatToChannels(desc.format));
		CreateTexture(pixels, desc);

		delete[] pixels;
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, std::vector<unsigned char>& pixels, const TextureDesc& desc) : ITexture(x, y)
	{
		CreateTexture(&pixels[0], desc);
	}

	Texture::~Texture()
	{
		DestroyTexture();
	}

	void Texture::Redefine(const unsigned int& x, const unsigned int& y, const TextureDesc& desc)
	{
		DestroyTexture();

		auto pixels = GenerateColourTexture(x, y, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
		CreateTexture(pixels, desc);

		delete[] pixels;
	}

	void Texture::CreateTexture(unsigned char* data, const TextureDesc& desc)
	{
		CreateTextureObject();
		Bind();

		this->format = desc.format;

		const int channels = FormatToChannels(desc.format);
		const glm::vec4 invert = glm::vec4(
			BE::Math::IsBitSet((int)desc.flags, 0),
			BE::Math::IsBitSet((int)desc.flags, 1),
			BE::Math::IsBitSet((int)desc.flags, 2),
			BE::Math::IsBitSet((int)desc.flags, 3)
		);

		if (glm::length(invert) > 0) {
			int i = 0;
			while (i < (sizeX * sizeY) * channels) {
				if (invert.r > 0)
					data[i + 0] = 255 - data[i + 0];
				if (invert.g > 0)
					data[i + 1] = 255 - data[i + 1];
				if (invert.b > 0)
					data[i + 2] = 255 - data[i + 2];
				if (invert.a > 0)
					data[i + 3] = 255 - data[i + 3];

				i += channels;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)desc.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)desc.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)desc.filter); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)desc.filter);

		if (desc.wrapping == Wrapping::BORDER) {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, desc.borderColour);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)desc.format, sizeX, sizeY, 0, (GLint)desc.format, GL_UNSIGNED_BYTE, data);
	}

	stbi_uc* Texture::GenerateColourTexture(const unsigned int& size_x, const unsigned int& size_y, const glm::vec3& c1, const glm::vec3& c2, const unsigned int& channels) {
		BE_ASSERT(channels <= 4, "Too many channels");
		
		this->sizeX = size_x;
		this->sizeY = size_y;
		
		stbi_uc* data = new stbi_uc[sizeX * sizeY * channels];

		int index = 0;
		bool colSwitch = true;
		for (int y = 0; y < size_y; y++) {
			for (int x = 0; x < size_x; x++) {
				if (colSwitch) {
					data[index++] = (stbi_uc)c1.r;
			
					if(channels >= 2)
						data[index++] = (stbi_uc)c1.g;

					if (channels >= 3)
						data[index++] = (stbi_uc)c1.b;
				}
				else {
					data[index++] = 0;
					if (channels >= 2)
						data[index++] = 0;
					if (channels >= 3)
						data[index++] = 0;
				}

				if (channels >= 4)
					data[index++] = 255;

				colSwitch = (((x + 1) % 8) == 0) ? !colSwitch : colSwitch;
			}

			colSwitch = (((y + 1) % 8) == 0) ? !colSwitch : colSwitch;
		}

		return data;
	}
}
