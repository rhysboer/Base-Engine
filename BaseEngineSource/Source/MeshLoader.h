#pragma once
#include "Mesh.h"
#include <unordered_map>

namespace BE::MeshLoader {
	enum class MeshFlags
	{
		IGNORE_TANGENT_SPACE = 1 << 0,
		IMPORT_COLOUR = 1 << 1,
	};

	Mesh* LoadFile(const char* path, const unsigned int& flags = 0);

	Mesh* CreateCube(const float& size = 1);
	Mesh* CreatePlane(const float& size = 1); // TODO
	Mesh* CreateQuad(const float& size = 1);
	Mesh* CreateSphere(const float& radius = 1);
	Mesh* CreateIcosphere(); // TODO
}