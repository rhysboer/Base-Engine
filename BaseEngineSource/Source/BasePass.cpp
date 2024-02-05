#include "BasePass.h"
#include "MeshRenderer.h"
#include "GL.h"

namespace BE {

	BasePass::BasePass(const char* debugName, const PassTag& tags, Framebuffer* buffer) 
		: buffer(buffer), IRenderPass(debugName, tags)
	{ }

	void BasePass::OnRender(std::vector<MeshRenderer*> meshes)
	{
		if (buffer == nullptr)
			return;

		Mesh* mesh = nullptr;
		for (int i = 0; i < meshes.size(); i++) {
			mesh = meshes[i]->GetMesh();

			if (mesh == nullptr)
				continue;

			for (int j = 0; j < mesh->GetMeshCount(); j++) {
				if (!meshes[i]->CanRender())
					continue;

				meshes[i]->BindMaterial(j);

				GL::DrawMesh(*mesh, j);
			}
		}
	}
}