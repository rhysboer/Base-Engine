#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Entity.h"
#include "IComponent.h"

namespace BE {
	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	{ 
		this->mesh = mesh;
		this->material = material;
	}

	void MeshRenderer::OnStart() {
	}

	void MeshRenderer::OnDestroy() {
	}

	void MeshRenderer::OnRender(Camera* const camera) {
		mesh->BindVAO();
		
		material->SetUniform("model", GetEntity()->transform.ModelMatrix());
		material->UpdateShaders();
		
		

		if(!mesh->IsUsingEBO())
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount());
		else
			glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}