#include "Application.h"

void Application::OnStart() {
	camera = new Camera3D(glm::vec3(0, 0, 10));
	object = CreateObject3D::FromOBJFile(glm::vec3(0), "C:\\Users\\Rhys\\Desktop\\Tribes\\Tile.obj");
	
	object->SetShader(ShaderManager::AddShader("Default", "./data/shaders/default/vertexColour.vert", "./data/shaders/default/vertexColour.frag"));
}

void Application::OnEnd() {
}

void Application::OnUpdate() {
	//object->GetTransform().Rotate(glm::vec3(1, 0, 0.5f), 1.0f);
}

void Application::OnRender() {
	object->Render(*camera);
}
