#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "ITexture.h"

namespace BE {
	class Texture : public ITexture
	{
	public:
		Texture(const std::string& imageData, const TextureDesc& desc = TextureDesc());
		Texture(const unsigned int& x, const unsigned int& y, const TextureDesc& desc = TextureDesc());
		Texture(const unsigned int& x, const unsigned int& y, const glm::vec3& colour, const TextureDesc& desc = TextureDesc());
		Texture(const unsigned int& x, const unsigned int& y, std::vector<unsigned char>& pixels, const TextureDesc& desc = TextureDesc());
		virtual ~Texture() override;

		inline glm::vec2 GetSize() const { return glm::vec2(x, y); }

		/// <summary>
		/// Loads texture from a path
		/// </summary>
		//static Texture* LoadTexture(const std::string& path, const TextureDesc& desc = TextureDesc());

		void Redefine(const unsigned int& x, const unsigned int& y, const TextureDesc& desc = TextureDesc());

	protected:

		void CreateTexture(unsigned char* data, const TextureDesc& desc = TextureDesc());

		//unsigned int id;
		int x, y;

	private:

		unsigned char* GenerateColourTexture(const unsigned int& x, const unsigned int& y, const glm::vec3& c1, const glm::vec3& c2);
	};
}

