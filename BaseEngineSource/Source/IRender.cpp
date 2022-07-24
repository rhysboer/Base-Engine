#include "IRender.h"
#include "Entity.h"
#include "Material.h"

#include "imgui.h"
#include "Mesh.h"

namespace BE {
	IRender::IRender() : mesh(nullptr), materials(std::vector<Material*>()) {
	}

	IRender::~IRender() {
	}

	void IRender::SetMaterials(const std::vector<Material*> materials)
	{

	}

	void IRender::OnComponentDraw()
	{
		ImGui::Text("Mesh Count: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(mesh->GetMeshCount()).c_str());
	}
}