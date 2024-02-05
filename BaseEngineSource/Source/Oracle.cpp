#ifdef BE_DEBUG
#pragma warning(disable : 26812) // Disable non enum class warning
#include "Oracle.h"
#include <string>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "imgui.h"
#include "Scene.h"
#include "Entity.h"
#include "BEMath.h"
#include "Gizmos.h"
#include "MeshRenderer.h"
#include "BoundingBox.h"

namespace BE
{
	glm::vec3 Oracle::selectedEntityEuler = { 0, 0, 0 };
	std::vector<Oracle::Tab> Oracle::tabs = std::vector<Oracle::Tab>();
	Entity* Oracle::selectedEntity = nullptr;
	BE::Oracle::OracleSettings Oracle::settings = {};

	void Oracle::Init()
	{
		tabs.push_back(Tab("Scene", SceneUI));
		tabs.push_back(Tab("Rendering", RenderingUI));
		tabs.push_back(Tab("Settings", SettingsUI));
	}


	void BE::Oracle::Draw()
	{
		static bool isOpen = true;

		ImGui::ShowDemoWindow();
		
		ImGui::Begin("Oracle");
		ImGui::BeginTabBar("OracleTabs", ImGuiTabBarFlags_None);

		for (int i = 0; i < tabs.size(); i++)
		{
			if (ImGui::BeginTabItem(tabs[i].name))
			{
				tabs[i].func();
				ImGui::EndTabItem();
			}
		}


		ImGui::EndTabBar();
		ImGui::End();
	}

	void Oracle::SceneUI()
	{

		Scene* scene = BE::Scene::GetActiveScene();

		std::vector<Entity*> entities = std::vector<Entity*>();

		auto iter = scene->GetEntityManager().GetEntityIterBegin();
		while (iter != scene->GetEntityManager().GetEntityIterEnd())
		{
			Entity* entity = (*iter).second;

			if (entity->transform.GetParent() == nullptr)
			{
				entities.push_back((*iter).second);
			}

			++iter;
		}

		ImGui::Text("Scene Hierarchy");

		float width = ImGui::GetColumnWidth();
		if (ImGui::BeginChild("Scene Hierarchy", { width * 0.4f, 0 }, true))
		{
			for (int i = 0; i < entities.size(); i++)
			{
				SceneUI_AddToSceneHierarchy(entities[i]);
			}
		
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("Selected Entity", {}, true);
		if (selectedEntity != nullptr)
		{
			bool isEnabled = selectedEntity->IsActive();
		
			ImGui::Text("Entity: %s", selectedEntity->GetName().c_str());
			ImGui::SameLine(ImGui::GetColumnWidth() - 60);
			ImGui::Checkbox("Toggle", &isEnabled);
		
			selectedEntity->SetEnabled(isEnabled);
		
			ImGui::Separator();
		
			glm::vec3 position = selectedEntity->transform.GetPosition();
			glm::vec3 scale = selectedEntity->transform.GetScale();
			glm::vec3 euler = glm::degrees(selectedEntity->transform.GetEuler());
		
			ImGui::Text("Position");
			ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f);
			ImGui::Text("Rotation");
			ImGui::DragFloat3("##Rotation", glm::value_ptr(euler), 0.1f);
			ImGui::Text("Scale");
			ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);
		
			selectedEntity->transform.SetPosition(position);
			selectedEntity->transform.SetRotation(glm::radians(euler));
			selectedEntity->transform.SetScale(scale);
		
			ImGui::Separator();
			ImGui::Text("Components:");
		
			std::vector<IComponent*> components = std::vector<IComponent*>();
			selectedEntity->GetComponents(components);
			
			for (int i = 0; i < components.size(); i++)
			{
				if (ImGui::CollapsingHeader(components[i]->GetName()))
				{
					ImGui::BeginChild(components[i]->GetName(), {0, 0}, true);
					components[i]->OnComponentDraw();
					ImGui::EndChild();
				}
			}
		
			// Draw Transform
			if(settings.drawTransform)
				Gizmos::DrawTransform(selectedEntity->transform.ModelMatrix(), 0.25f);
		
			// Draw Bounding Box
			BE::MeshRenderer* renderer = selectedEntity->GetComponent<BE::MeshRenderer>();
			if (settings.drawBoundingBox && renderer != nullptr) {
				BE::Gizmos::SetColour(COLOUR_RED);
				BE::Gizmos::DrawAABB(*renderer->GetMesh()->GetBounds(), selectedEntity->transform.ModelMatrix());
				BE::Gizmos::RevertColour();
			}
		
			// Draw Bone
			if (settings.drawBones && renderer != nullptr) {
				auto bones = renderer->GetMesh()->GetMeshData(0)->boneOffsets;
		
				for (int i = 0; i < bones.size(); i++) {
					BE::Gizmos::DrawTransform(selectedEntity->transform.ModelMatrix() * bones[i], 0.25f);
				}
			}
		}
		ImGui::EndChild();
	}

	void Oracle::SceneUI_AddToSceneHierarchy(Entity* entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick |
			(entity->transform.GetChildCount() > 0 ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf) | (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx(entity->GetName().c_str(), flags);

		if (ImGui::IsItemClicked() && selectedEntity != entity)
		{
			selectedEntity = entity;
			selectedEntityEuler = glm::degrees(glm::eulerAngles(selectedEntity->transform.GetRotation()));
		}

		if (opened)
		{
			for (int i = 0; i < entity->transform.GetChildCount(); i++)
			{
				SceneUI_AddToSceneHierarchy(entity->transform.GetChild(i));
			}

			ImGui::TreePop();
		}
	}

	void Oracle::RenderingUI()
	{
		ImGui::Text("Stats Here");
	}

	void Oracle::SettingsUI()
	{
		ImGui::Checkbox("Draw Transform", &settings.drawTransform);
		ImGui::Checkbox("Draw Bones", &settings.drawBones);
		ImGui::Checkbox("Draw Bounding Box", &settings.drawBoundingBox);
	}
}

#endif