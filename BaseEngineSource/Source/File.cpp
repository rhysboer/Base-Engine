#include "File.h"
#include <fstream>
#include <sstream>
#include <direct.h>
#include "glm/glm.hpp"
#include "Logging.h"

namespace BE {
	std::string File::LoadFile(const std::string& path) {
		return LoadFile(path.c_str());
	}

	std::string File::LoadFile(const char* path) {
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open()) {
			BE_WARNING("Failed to find file at path: %s", path);
			return "";
		}

		auto ss = std::ostringstream{};
		ss << file.rdbuf();
		file.close();

		return ss.str();
	}

	std::string File::GetFileExtension(const char* file)
	{
		size_t len = strlen(file);

		// Max search depth, most file extensions are no longer than 3 characters
		const int depth = len - glm::min((int)len, 8);

		for (size_t i = len - 1; i >= depth; --i) {
			if (file[i] == '.') {
				return std::string(file + i + 1, len - i);
			}
		}

		return "";
	}

	void File::LoadAllFilesByExtension(std::vector<std::string>& files, const char* path, const char* extension)
	{
	

	}
}