#include "Application.h"

void Application::OnStart() {
	//camera = new Camera3D(glm::vec3(0, 5, 10));
	camera = new Camera(glm::vec3(0, 0, 20));

	glm::vec2 size = Engine::GetWindowSize();

	//camera->CreatePerspective(60.0f);
	camera->CreateOrthographic(-size.x / 2.0f, size.x / 2.0f, -size.y / 2.0f, size.y / 2.0f);
	camera->SetZoom(20);
	//camera->CreateOrthographic(-10, 10, -10, 10);

	a = CreateObject3D::FromOBJFile(glm::vec3(0), "C:\\Users\\Rhys\\Desktop\\Tribes\\Tile.obj");
	b = CreateObject3D::FromOBJFile(glm::vec3(5, 0, 0), "C:\\Users\\Rhys\\Desktop\\Tribes\\Town_R.obj");
	
	a->SetShader(ShaderManager::AddShader("Default", "./data/shaders/default/vertexColour.vert", "./data/shaders/default/vertexColour.frag"));
	b->SetShader(ShaderManager::GetShader("Default"));
}

void Application::OnEnd() {
}

void Application::OnUpdate() {

	if(Input::IsKeyDown(GLFW_KEY_D))
		camera->GetTransform().RotateY(0.1f);
	if(Input::IsKeyDown(GLFW_KEY_A))
		camera->GetTransform().RotateY(-0.1f);


	//camera->GetTransform().RotateAroundPoint(glm::vec3(0), 1.0f, glm::vec3(0, 1, 0));

	//ImGui::Begin("Camera");
	//ImGui::Text("UP: X:%f, Y:%f, Z:%f", camera->GetTransform().GetUp().x, camera->GetTransform().GetUp().y, camera->GetTransform().GetUp().z);
	//ImGui::End();
}

void Application::OnRender() {
	a->Render(*camera);
	b->Render(*camera);
}
