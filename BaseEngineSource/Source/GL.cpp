#include "GL.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"

namespace BE::GL {
	void DrawMesh(const Mesh& mesh, const unsigned int& subMesh)
	{
		mesh.BindVAO(subMesh);

		if (!mesh.HasEBO(subMesh))
			DrawArrays(GL_TRIANGLES, 0, mesh.GetIndicesCount(subMesh));
		else
			DrawElements(GL_TRIANGLES, mesh.GetIndicesCount(subMesh), GL_UNSIGNED_INT, 0);
	}

	void DrawArrays(const unsigned int& mode, const int& first, const int& count)
	{
		glDrawArrays(mode, first, count);
	}

	void DrawElements(const unsigned int& mode, const int& count, const unsigned int& type, const void* indices)
	{
		glDrawElements(mode, count, type, indices);
	}

	void SetFaceCulling(const Face& face)
	{
		glCullFace((int)face == 0 ? GL_FRONT : GL_BACK);
	}

	void WireFrame(const bool& enabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
	}
}


