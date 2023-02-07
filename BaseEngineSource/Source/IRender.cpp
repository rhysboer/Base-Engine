#include "IRender.h"
#include "Entity.h"
#include "Material.h"
#include "Renderer.h"

#include "imgui.h"
#include "Mesh.h"

namespace BE {
	IRender::IRender() : mesh(nullptr), materials(std::vector<Material*>()) {
		BE::Renderer::SetDirty();
	}

	IRender::~IRender() {
	}

	void IRender::SetMaterials(const std::vector<Material*> materials)
	{
		// TODO
	}

	void IRender::SetMesh(BE::Mesh* mesh)
	{
		if (this->mesh != nullptr) {
			delete this->mesh;
			this->mesh = nullptr;
		}

		this->mesh = mesh;
	}

	void IRender::SetMesh(BE::MeshData& mesh)
	{
		SetMesh(new Mesh(mesh));
	}

	void IRender::OnComponentDraw()
	{
		ImGui::Text("Mesh Count: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(mesh->GetMeshCount()).c_str());
	}
}