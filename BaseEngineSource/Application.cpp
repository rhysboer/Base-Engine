#include "Application.h"

void Application::OnStart() {
	camera = new Camera3D(glm::vec3(0, 0, 10));
	object = Object3D::CreateObject_Cube(glm::vec3(0));
	
	object->SetShader(ShaderManager::AddShader("Default", "./data/shaders/default/default.vert", "./data/shaders/default/default.frag"));
}

void Application::OnEnd() {
}

void Application::OnUpdate() {

}

void Application::OnRender() {
	object->Render(*camera);
}
