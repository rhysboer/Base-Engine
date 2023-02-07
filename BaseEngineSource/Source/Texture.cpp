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
	Texture::Texture(const std::string& imageData, const TextureDesc& desc)
	{
		int channels;

		// Flip if opengl
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load_from_memory((unsigned char*)imageData.c_str(), imageData.length(), &x, &y, &channels, FormatToChannels(desc.format));

		if (data == nullptr) {
			Logging::Error("Failed to load texture");
			data = GenerateColourTexture(64, 64, ERROR_COL_ONE, ERROR_COL_TWO);
		}

		CreateTexture(data, desc);
		stbi_image_free(data);
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const TextureDesc& desc) : x(x), y(y)
	{
		auto pixels = GenerateColourTexture(x, y, glm::vec3(128, 128, 128), glm::vec3(255, 255, 255));
		CreateTexture(pixels, desc);

		delete[] pixels;
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const glm::vec3& colour, const TextureDesc& desc) : x(x), y(y)
	{
		auto pixels = GenerateColourTexture(x, y, colour, colour);
		CreateTexture(pixels, desc);

		delete[] pixels;
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, std::vector<unsigned char>& pixels, const TextureDesc& desc) : x(x), y(y)
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

		const int channels = FormatToChannels(desc.format);
		const glm::vec4 invert = glm::vec4(
			BE::Math::IsBitSet((int)desc.flags, 0),
			BE::Math::IsBitSet((int)desc.flags, 1),
			BE::Math::IsBitSet((int)desc.flags, 2),
			BE::Math::IsBitSet((int)desc.flags, 3)
		);

		if (glm::length(invert) > 0) {
			int i = 0;
			while (i < (x * y) * channels) {
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)desc.format, x, y, 0, (GLint)desc.format, GL_UNSIGNED_BYTE, data);
	}

	stbi_uc* Texture::GenerateColourTexture(const unsigned int& size_x, const unsigned int& size_y, const glm::vec3& c1, const glm::vec3& c2) {
		this->x = size_x;
		this->y = size_y;
		
		stbi_uc* data = new stbi_uc[x * y * 4];

		int index = 0;
		bool colSwitch = true;
		for (int y = 0; y < size_y; y++) {
			for (int x = 0; x < size_x; x++) {
				if (colSwitch) {
					data[index++] = (stbi_uc)c1.r;
					data[index++] = (stbi_uc)c1.g;
					data[index++] = (stbi_uc)c1.b;
				}
				else {
					data[index++] = 0;
					data[index++] = 0;
					data[index++] = 0;
				}

				data[index++] = 255;
				colSwitch = (((x + 1) % 16) == 0) ? !colSwitch : colSwitch;
			}

			colSwitch = (((y + 1) % 16) == 0) ? !colSwitch : colSwitch;
		}

		return data;
	}
}
