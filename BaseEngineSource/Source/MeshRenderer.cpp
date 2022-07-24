#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Entity.h"
#include "IComponent.h"
#include "BEGlobal.h"

namespace BE {
	MeshRenderer::MeshRenderer()
		: MeshRenderer::MeshRenderer(nullptr, BEGlobal::GetDefaultMaterial()) { }

	MeshRenderer::MeshRenderer(Mesh* mesh)
		: MeshRenderer::MeshRenderer(mesh, BEGlobal::GetDefaultMaterial()) { }

	MeshRenderer::MeshRenderer(Material* material)
		: MeshRenderer::MeshRenderer(nullptr, material) { }

	MeshRenderer::MeshRenderer(Mesh* mesh, std::vector<Material*> materials)
		: MeshRenderer::MeshRenderer(mesh, &materials[0], materials.size()) { }

	MeshRenderer::MeshRenderer(std::vector<Material*> materials)
		: MeshRenderer::MeshRenderer(nullptr, &materials[0], materials.size()) { }

	MeshRenderer::MeshRenderer(Material** materials, const unsigned int& size)
		: MeshRenderer::MeshRenderer(nullptr, materials, size) {}

	MeshRenderer::MeshRenderer(Mesh* mesh, Material** materials, const unsigned int& size)
	{
		this->mesh = mesh;
		this->materials = std::vector<Material*>(size);

		for (int i = 0; i < size; i++)
			this->materials[i] = materials[i];
	}

	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	{
		this->mesh = mesh;
		this->materials = std::vector<Material*>({ material });
	}

	void MeshRenderer::OnRender(const unsigned int& subMeshIndex) {
		Material* material = GetMaterial(subMeshIndex);
		if (material == nullptr)
			material = BEGlobal::GetDefaultMaterial();

		material->SetUniform("model", GetEntity()->transform.ModelMatrix());
		material->UpdateShaders();
	
		mesh->BindVAO(subMeshIndex);

		if(!mesh->HasEBO(subMeshIndex))
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount(subMeshIndex));
		else
			glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(subMeshIndex), GL_UNSIGNED_INT, 0);
	}
}