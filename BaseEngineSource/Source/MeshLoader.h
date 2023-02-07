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

	MeshData CreateCube(const float& size = 1);
	MeshData CreatePlane(const float& size = 1); // TODO
	MeshData CreateQuad(const float& size = 1);
	MeshData CreateSphere(const float& radius = 1);
	MeshData CreateIcosphere(); // TODO
}