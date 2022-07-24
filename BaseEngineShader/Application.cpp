#include "Application.h"
#include "imgui.h"
#include "Logging.h"

#include "Camera.h"
#include "Entity.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Input.h"


void Application::OnEngineInit()
{
	auto* scene = BE::Scene::CreateScene("Graph");

	auto* camera = scene->GetEntityManager().CreateEntity("Camera");
	camera->AddComponent<BE::Camera>();
	camera->transform.SetPosition(0, 0, 10);


}

void Application::OnEngineDestroy()
{

}

void Application::OnEngineUpdate() 
{
	glm::vec2 mousePos = BE::Input::MousePosition();

	BE::Gizmos::DrawCube(0.5f);

	auto* camera = BE::Scene::GetScene("Graph")->GetEntityManager().GetEntity("Camera")->GetComponent<BE::Camera>();


	ImGui::Begin("Node", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Button("Input");
	ImGui::SameLine(150.0f);

	ImVec2 pos = ImGui::GetWindowPos();
	ImGui::Text("%f, %f", pos.x, pos.y);
	ImGui::Text("%f, %f", mousePos.x, mousePos.y);
	ImGui::Button("Output");
	ImGui::End();
	
	BE::Gizmos::DrawLine(camera->ScreenSpaceToWorldSpace(glm::vec3(pos.x, pos.y, 0)), camera->ScreenSpaceToWorldSpace(mousePos));
}