#pragma once

namespace BE {
	class Mesh;
}

namespace BE::GL {
	enum class Face
	{
		FRONT_FACE,
		BACK_FACE
	};

	void DrawMesh(const Mesh& mesh, const unsigned int& subMesh = 0);

	void DrawArrays(const unsigned int& mode, const int& first, const int& count);
	void DrawElements(const unsigned int& mode, const int& count, const unsigned int& type, const void* indices);

	void SetFaceCulling(const Face& face);

	void WireFrame(const bool& enabled);
}