#pragma once
#include "Mesh.h"
#include <string>

namespace BE::Loader::OBJ {
	BE::Mesh* Load(const std::string& data, const unsigned int& flags = 0);
}

