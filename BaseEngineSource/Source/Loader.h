#include <vector>
#include "tiny_obj_loader.h"

namespace Loader {
	bool LoadOBJ(const char* path, std::vector<float>& vertexData, std::vector<unsigned int>& vertexAttributes);
}