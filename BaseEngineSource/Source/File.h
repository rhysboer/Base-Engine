#pragma once
#include <string>
#include <vector>

namespace BE::File {
	std::string LoadFile(const std::string& path);
	std::string LoadFile(const char* path);

	std::string GetFileExtension(const std::string& file);
	std::string GetFileExtension(const char* file);

	void LoadAllFilesByExtension(std::vector<std::string>& files, const char* path, const char* extension);
}
