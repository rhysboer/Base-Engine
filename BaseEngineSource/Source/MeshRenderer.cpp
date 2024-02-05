#include "MeshRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Material.h"
#include "Mesh.h"
#include "Entity.h"
#include "IComponent.h"
#include "BEGlobal.h"

#include "imgui.h"

#ifdef BE_DEBUG
#include "Uniform.h"
#include "Renderer.h"
#endif

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
		: MeshRenderer::MeshRenderer(mesh, &material, 1) {}

	MeshRenderer::MeshRenderer(const MeshData& mesh)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), BEGlobal::GetDefaultMaterial()) {}

	MeshRenderer::MeshRenderer(const MeshData& mesh, Material* material)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), material) {}

	MeshRenderer::MeshRenderer(const MeshData& mesh, std::vector<Material*> materials)
		: MeshRenderer::MeshRenderer(new Mesh(mesh), &materials[0], materials.size()) {}

	MeshRenderer::MeshRenderer(Mesh* mesh, Material** const materials, const unsigned int& size)
	{
		this->mesh = mesh;
		this->passTags = PassTags::GetDefaultTag();
		this->materials = std::vector<Material*>(size);

		for (int i = 0; i < size; i++)
			this->materials[i] = materials[i];
	}

	//MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	//{
	//	this->mesh = mesh;
	//	this->materials = std::vector<Material*>({ material });
	//}


	//void MeshRenderer::OnRender() {
		//if (mesh == nullptr)
		//	return;
		//
		//for (int i = 0; i < mesh->GetMeshCount(); i++) {
		//	Material* material = GetMaterial(i);
		//	if (material == nullptr)
		//		material = BEGlobal::GetDefaultMaterial();
		//
		//	material->SetUniform("model", GetEntity()->transform.ModelMatrix());
		//	material->Use();
		//
		//	mesh->BindVAO(i);
		//
		//	if(!mesh->HasEBO(i))
		//		glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount(i));
		//	else
		//		glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(i), GL_UNSIGNED_INT, 0);
		//}
	//}

	void MeshRenderer::BindMaterial(const unsigned int& subMesh) const
	{
		Material* material = GetMaterial(subMesh);
		if (material == nullptr)
			material = BEGlobal::GetDefaultMaterial();

		material->SetUniform("model", GetEntity()->transform.ModelMatrix());
		material->Use();
	}

	void MeshRenderer::SetMaterial(Material& material, const unsigned int& index)
	{
		if (index >= materials.size()) {
			BE_WARNING("Index out of range");
			return;
		}

		materials[index] = &material;
	}

	void MeshRenderer::SetMaterials(const std::vector<Material*> materials)
	{
		// TODO
	}
	
	void MeshRenderer::SetMesh(BE::Mesh* mesh)
	{
		if (this->mesh != nullptr) {
			delete this->mesh;
			this->mesh = nullptr;
		}
	
		this->mesh = mesh;
	}
	
	void MeshRenderer::SetMesh(BE::MeshData& mesh)
	{
		SetMesh(new Mesh(mesh));
	}

#ifdef BE_DEBUG
	void MeshRenderer::OnComponentDraw()
	{
		bool valuesChanged = false;
		
		int priority = renderPriority;

		ImGui::Checkbox("Render", &render);
		ImGui::Checkbox("Cast Shadows", &castShadows);
		if (ImGui::InputInt("Render Priority", &priority))
		{
			renderPriority = glm::clamp(priority, SHRT_MIN, SHRT_MAX);
			valuesChanged = true;
		}

		std::vector<std::string> tags = PassTags::GetAllTags();
		bool isSelected[32];

		// TODO: use stringstream or whatever it is
		std::string tag = "";

		for (int i = 0; i < tags.size(); i++)
		{
			if (PassTags::HasTag(passTags, 1 << i))
			{
				if (tag.size() != 0)
					tag += ", " + tags[i];
				else
					tag += tags[i];

				isSelected[i] = true;
			}
			else
			{
				isSelected[i] = false;
			}
		}

		if (ImGui::BeginCombo("PassTags", tag.c_str()))
		{
			for (int i = 0; i < tags.size(); i++)
			{
				if (ImGui::Selectable(tags[i].c_str(), &isSelected[i]))
				{
					PassTag newTag = PassTags::GetTag(tags[i].c_str());
					passTags = isSelected[i] ? passTags | newTag : passTags ^ newTag;
					valuesChanged = true;
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::CollapsingHeader("Materials")) {
			if (ImGui::BeginChild("Materials", {}, true)) {
				for (int i = 0; i < materials.size(); i++)
				{
					Material* mat = materials[i];

					if (ImGui::CollapsingHeader(mat->GetShader()->GetName().c_str()))
					{
						bool depthWrite = mat->GetDepthWrite();

						if (ImGui::Checkbox("Depth Write", &depthWrite))
						{
							mat->SetDepthWrite(depthWrite);
							valuesChanged = true;
						}

						for (int j = 0; j < mat->GetUniformCount(); j++)
						{
							Uniform* uniform = mat->GetUniform(j);

							if (uniform->IsBEInternal())
								continue;

							switch (uniform->GetType())
							{
							case UniformType::BOOLEAN:
							{
								bool value = (int)uniform->GetValue(0);

								if (ImGui::Checkbox(uniform->GetName(), &value))
								{
									int v = value;
									uniform->SetValue(&v, 1);
									valuesChanged = true;
								}

								break;
							}
							case UniformType::FLOAT:
							{
								float value = *(float*)uniform->GetValue(0);

								if (ImGui::DragFloat(uniform->GetName(), &value, 0.1f))
								{
									uniform->SetValue(&value, 1);
									valuesChanged = true;
								}

								break;
							}
							case UniformType::INT:
								//int output = (int)uniform->GetValue(0);
								ImGui::Text("INT");
								break;
							case UniformType::TEXTURE_2D:
							{
								float size = ImGui::GetColumnWidth() / 2.5f;
								ImGui::BeginChild("Textures", { 0, size });
								ITexture* texture = (ITexture*)uniform->GetValue(0);

								ImGui::Image((void*)texture->GetID(), { size, size }, { 0, 1 }, { 1, 0 });
								ImGui::SameLine();
								ImGui::Text(uniform->GetName());
								ImGui::EndChild();
								break;
							}
							case UniformType::VECTOR_2:
							{
								glm::vec2 value = glm::vec2(*(float*)uniform->GetValue(0), *(float*)uniform->GetValue(1));

								if (ImGui::DragFloat2(uniform->GetName(), &value[0], 0.1f))
								{
									uniform->SetValue(&value, 2);
									valuesChanged = true;
								}

								break;
							}
							case UniformType::VECTOR_3:
								ImGui::Text("VEC3");
								break;
							default:
								ImGui::Text(uniform->GetName());
								break;
							}
						}
					}
				}
				ImGui::EndChild();
			}
		}

		if (valuesChanged)
		{
			BE::Renderer::SetDirty();
		}
	}
#endif
}