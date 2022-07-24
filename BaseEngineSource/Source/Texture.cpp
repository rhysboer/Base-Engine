#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Texture.h"
#include "stb_image.h"
#include "Logging.h"
#include <glad/glad.h>

#define ERROR_COL_ONE glm::vec3(255, 0, 255)
#define ERROR_COL_TWO glm::vec3(0, 0, 0)

namespace BE {
	Texture::Texture(const std::string& imageData, const TextureFlags& flags)
	{
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load_from_memory((unsigned char*)imageData.c_str(), imageData.length(), &x, &y, &format, 0);

		if (data == nullptr) {
			Logging::Error("Failed to load texture");
			data = GenerateColourTexture(64, 64, ERROR_COL_ONE, ERROR_COL_TWO);
		}

		CreateTexture(data, (int)flags);
		stbi_image_free(data);
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const TextureFlags& flags) : x(x), y(y), id(0), format(0)
	{
		CreateTexture(GenerateColourTexture(x, y, glm::vec3(128, 128, 128), glm::vec3(255, 255, 255)), (int)flags);
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, const glm::vec3& colour, const TextureFlags& flags) : x(x), y(y), id(0), format(0)
	{
		CreateTexture(GenerateColourTexture(x, y, colour, colour), (int)flags);
	}

	Texture::Texture(const unsigned int& x, const unsigned int& y, std::vector<unsigned char>& pixels, const TextureFlags& flags) : x(x), y(y), id(0), format(0)
	{
		format = (pixels.size() == x * y * 4) ? 4 : 3;

		// TODO: Check X & Y range, make sure it fits with the pixel array
		CreateTexture(&pixels[0], (int)flags);
	}

	void Texture::CreateTexture(unsigned char* data, const int& flags)
	{
		GenTextureID();
		Bind();

		glm::vec3 invert = { (flags >> (int)TextureFlags::INVERT_R - 1) & 1, (flags >> (int)TextureFlags::INVERT_G - 1) & 1, (flags >> (int)TextureFlags::INVERT_B - 1) & 1 };
		if (glm::length(invert) > 0) {
			int i = 0;
			while (i < (x * y) * 4) {
				if (invert.r > 0)
					data[i + 0] = 255 - data[i + 0];
				if (invert.g > 0)
					data[i + 1] = 255 - data[i + 1];
				if (invert.b > 0)
					data[i + 2] = 255 - data[i + 2];
				i += 4;// format;
			}
		}
		
		format = GetFormat(format);
		

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
	}

	stbi_uc* Texture::GenerateColourTexture(const unsigned int& size_x, const unsigned int& size_y, const glm::vec3& c1, const glm::vec3& c2) {
		this->x = size_x;
		this->y = size_y;
		format = 4;
		
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

	int Texture::GetFormat(const int& format) const
	{
		switch (format) {
			case 1: return GL_RED;
			case 2: return GL_RG;
			case 3: return GL_RGB;
			case 4: return GL_RGBA;
			default:
				assert(false && "Texture format not supported");
		}
	}
}
