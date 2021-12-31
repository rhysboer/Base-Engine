#pragma once
#include "Mesh.h"

namespace BE::MeshUtil {
	Mesh* CreateCube(const float& size = 1);
	Mesh* CreatePlane(const float& size = 1); // TODO
	Mesh* CreateQuad(); // TODO
	Mesh* CreateSphere(); // TODO
	Mesh* CreateIcosphere(); // TODO
}

