#include "Application.h"

void Application::OnStart() {
	camera = new Camera3D(glm::vec3(0, 0, 10));
	object = Object3D::CreateObject_Cube(glm::vec3(0));
	
	object->SetShader(ShaderManager::AddShader("Default", "./data/shaders/default/default.vert", "./data/shaders/default/default.frag"));
}

void Application::OnEnd() {
}

void Application::OnUpdate() {
	object->GetTransform().Rotate(glm::vec3(1, 0, 0.5f), 1.0f);
}

void Application::OnRender() {
	object->Render(*camera);
}
