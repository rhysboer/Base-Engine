#include <vector>
#include "tiny_obj_loader.h"

namespace BE::FileLoader {
	bool LoadOBJ(const char* path, std::vector<float>& vertexData, std::vector<unsigned int>& vertexAttributes);
}