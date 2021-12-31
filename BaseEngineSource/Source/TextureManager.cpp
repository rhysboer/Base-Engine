#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include "TextureManager.h"
#include "Logger.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <assert.h>

#define IsValid(t) t > 0

namespace BE {
	std::vector<Texture> TextureManager::textures = std::vector<Texture>();
	std::unordered_map<const char*, unsigned int> TextureManager::textureIndexes = std::unordered_map<const char*, unsigned int>();
	unsigned int TextureManager::index = 0;

	Texture TextureManager::LoadTexture(const char* name, const char* path) {
		assert(textureIndexes.find(name) == textureIndexes.end() && "TextureManager: Texture with name already exists");

		Texture texture{};

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path, &texture.width, &texture.height, &texture.format, 0);

		if(data == nullptr) {
			Logger::Error("TextureManager: Failed to load texture - ", name);
			data = GenerateErrorTexture(texture);
		}

		glGenTextures(1, &texture.GLid);
		glBindTexture(GL_TEXTURE_2D, texture.GLid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, texture.GetFormat(), texture.width, texture.height, 0, texture.GetFormat(), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		// Store Texture Data
		textures.push_back(texture);
		textureIndexes.insert({name, index});
		++index;

		return texture;
	}

	Texture TextureManager::GetTexture(const char* name) {
		auto iter = textureIndexes.find(name);
		assert(iter == textureIndexes.end() && "TextureManager: Texture does not exist");
		
		return textures[(*iter).second];
	}

	//void DestroyTexture(const Texture& texture) {
	//	if(IsValid(texture))
	//		glDeleteTextures(1, &texture);
	//}

	void TextureManager::Bind(const unsigned int& id, const unsigned int& slot) {
		if(IsValid(id)) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
		}
	}

	stbi_uc* TextureManager::GenerateErrorTexture(Texture& texture) {
		const unsigned int size = 64;
		stbi_uc* data = new stbi_uc[size * size * 4];

		texture.height = size;
		texture.width = size;
		texture.format = 4;

		int index = 0;
		bool colSwitch = true;
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				if(colSwitch) {
					data[index++] = 255;
					data[index++] = 0;
					data[index++] = 255;
				} else {
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


	unsigned int Texture::GetFormat() const {
		switch(format) {
			case 1: return GL_RED;
			case 2: return GL_RG;
			case 3: return GL_RGB;
			case 4: return GL_RGBA;
		}

		assert(false && "Texture format not supported");
	}
}
