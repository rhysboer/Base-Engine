#include "EntityDebugger.h"
#include "imgui.h"

#include "Scene.h"
#include "EntityManager.h"
#include "Entity.h"
#include "glm/gtc/type_ptr.hpp"

#include "TimeManager.h"
#include "Logging.h"

namespace BE::Debug {
	EntityDebugger* EntityDebugger::instance = nullptr;

	void EntityDebugger::Update()
	{
		static glm::vec3 rot = glm::vec3(0, 0, 0);
		Scene* scene = Scene::GetScene("Main");

		if(scene == nullptr)
			return;

		ImGui::Begin("Entity Manager");		

		auto begin = scene->GetEntityManager().GetEntityIterBegin();
		auto end = scene->GetEntityManager().GetEntityIterEnd();
		auto curr = begin;
		
		unsigned int size = scene->GetEntityManager().GetEntityCount();


		Entity* entity = nullptr;

		ImGui::BeginChild("Entities", { 175, 0 }, true);
		for (int i = 0; curr != end; curr++, i++) {
			if (ImGui::Selectable(curr->second->GetName().c_str(), Get()->selectedEntity == i)) {
				Get()->selectedEntity = i;
				rot = glm::degrees(glm::eulerAngles(glm::normalize(curr->second->transform.GetRotation())));
			}

			if (Get()->selectedEntity == i)
				entity = curr->second;
		}
		ImGui::EndChild();
		
		ImGui::SameLine();
		ImGui::BeginChild("Selected", { 0, 0 }, true);
		if (entity != nullptr) {
			glm::vec3 pos = entity->transform.GetPosition();
			glm::vec3 scl = entity->transform.GetScale();
			//glm::vec3 rot = glm::degrees(glm::eulerAngles(entity->transform.GetRotation()));
		
			ImGui::Text(("Entity: " + entity->GetName()).c_str());
			ImGui::Separator();

			ImGui::Text("Position");
			ImGui::DragFloat3("##Position", glm::value_ptr(pos), 0.1f);
			ImGui::Text("Rotation");
			ImGui::DragFloat3("##Rotation", glm::value_ptr(rot), 0.25f);
			ImGui::Text("Scale");
			ImGui::DragFloat3("##Scale", glm::value_ptr(scl), 0.1f);
		
			glm::vec3 newRot = glm::radians(rot);
			entity->transform.SetPosition(pos);
			entity->transform.SetRotation(glm::identity<glm::quat>() * newRot);
			entity->transform.SetScale(scl);

			ImGui::Separator();
			// --- Components

			ImGui::Text("Components");
			ImGui::Separator();

			std::vector<IComponent*> components = std::vector<IComponent*>();
			entity->GetComponents(components);

			for (int i = 0; i < components.size(); i++) {
				if (ImGui::CollapsingHeader(components[i]->GetName())) {
					components[i]->OnComponentDraw();
				}
			}

		}
		ImGui::EndChild(); // Selected
		ImGui::End();

		//ImGui::ShowDemoWindow();
	}
}

