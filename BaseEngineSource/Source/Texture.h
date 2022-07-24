#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "ITexture.h"

namespace BE {
	enum class TextureFlags
	{
		NONE,
		INVERT_R = 1 << 0,
		INVERT_G = 1 << 1,
		INVERT_B = 1 << 2
	};

	class Texture : public ITexture
	{
	public:
		Texture(const std::string& imageData, const TextureFlags& flags = TextureFlags::NONE);
		Texture(const unsigned int& x, const unsigned int& y, const TextureFlags& flags = TextureFlags::NONE);
		Texture(const unsigned int& x, const unsigned int& y, const glm::vec3& colour, const TextureFlags& flags = TextureFlags::NONE);
		Texture(const unsigned int& x, const unsigned int& y, std::vector<unsigned char>& pixels, const TextureFlags& flags = TextureFlags::NONE);
		virtual ~Texture() override {}

		/// <summary>
		/// Loads texture from a path
		/// </summary>
		static Texture* LoadTexture(const std::string& path, const TextureFlags& flags = TextureFlags::NONE);

	protected:

		void CreateTexture(unsigned char* data, const int& flags = (int)TextureFlags::NONE);
		int GetFormat(const int& format) const;

		unsigned int id;
		int x, y, format;

	private:

		unsigned char* GenerateColourTexture(const unsigned int& x, const unsigned int& y, const glm::vec3& c1, const glm::vec3& c2);
	};
}

