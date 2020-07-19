#include "Application.h"

void Application::OnStart() {
	camera = new BE::FlyCamera(glm::vec3(0, 0, 20));

	glm::vec2 size = Engine::GetWindowSize();
	camera->CreatePerspective(60.0f);


	a = BE::Object3D::CreateFromOBJFile(glm::vec3(0), "./data/bunny.obj");
	a->SetShader(BE::ShaderManager::AddShader("default", "./data/shaders/default/default"));
	a->GetTransform().SetScale(2.5f);
}

void Application::OnEnd() {
}

void Application::OnUpdate() {
	camera->Update();


	//Gizmos::AddCamera(camera->ProjectionView());
	//
	//
	//AABB aabb = AABB(glm::vec3(0), glm::vec3(5));
	//Gizmos::DrawAABB(aabb, glm::vec3(0, 1, 0));
	//
	BE::Raycast::Ray ray;
	//camera->CreateRay(ray);
	//
	//RayCast::RayHit hit = RayCast::RayCastAABB(ray, aabb);
	//
	//Gizmos::DrawCube(hit.hitPosition, 0.2f, glm::vec3(1, 0, 0));


	//ImGui::Begin("Raycasting");
	//ImGui::Text("Is Ray Hitting AABB: ");
	//ImGui::SameLine();
	//if(hit.hit)
	//	ImGui::TextColored(ImVec4(0, 1, 0, 1), "True");
	//else
	//	ImGui::TextColored(ImVec4(1, 0, 0, 1), "False");
	//ImGui::End();

}

void Application::OnRender() {
	a->Render(*camera);
}