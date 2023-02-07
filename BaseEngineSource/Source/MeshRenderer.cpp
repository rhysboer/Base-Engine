#include "MeshRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
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

	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
		: MeshRenderer::MeshRenderer(mesh, &material, 1) { }

	MeshRenderer::MeshRenderer(Mesh* mesh, Material** const materials, const unsigned int& size)
	{
		this->mesh = mesh;
		this->materials = std::vector<Material*>(size);

		for (int i = 0; i < size; i++)
			this->materials[i] = materials[i];
	}

	MeshRenderer::MeshRenderer(const MeshData& mesh)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), BEGlobal::GetDefaultMaterial()) { }

	MeshRenderer::MeshRenderer(const MeshData& mesh, Material* material)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), material) { }

	MeshRenderer::MeshRenderer(const MeshData& mesh, std::vector<Material*> materials)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), &materials[0], materials.size()) { }

	//MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	//{
	//	this->mesh = mesh;
	//	this->materials = std::vector<Material*>({ material });
	//}


	void MeshRenderer::OnRender() {
		if (mesh == nullptr)
			return;

		for (int i = 0; i < mesh->GetMeshCount(); i++) {
			Material* material = GetMaterial(i);
			if (material == nullptr)
				material = BEGlobal::GetDefaultMaterial();

			material->SetUniform("model", GetEntity()->transform.ModelMatrix());
			material->Use();

			mesh->BindVAO(i);

			if(!mesh->HasEBO(i))
				glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount(i));
			else
				glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(i), GL_UNSIGNED_INT, 0);
		}
	}
}