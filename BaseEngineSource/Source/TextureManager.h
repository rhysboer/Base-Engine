#pragma once
#include <vector>
#include <unordered_map>

namespace BE {
	//struct Texture {
	//	unsigned int GLid, index;
	//	int width, height, format;
	//
	//	/*
	//	Format
	//	1           red
	//	2           red, alpha
	//	3           red, green, blue
	//	4           red, green, blue, alpha
	//	*/
	//	unsigned int GetFormat() const;
	//};
	//
	//class TextureManager {
	//public:
	//
	//	static Texture LoadTexture(const char* name, const char* path);
	//	static Texture GetTexture(const char* name);
	//
	//	static void Bind(const unsigned int& id, const unsigned int& slot);
	//private:
	//	TextureManager();
	//
	//	static unsigned char* GenerateErrorTexture(Texture& texture);
	//
	//	static std::vector<Texture> textures;
	//	static std::unordered_map<const char*, unsigned int> textureIndexes;
	//	static unsigned int index;
	//};
}

