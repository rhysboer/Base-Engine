#include "Application.h"
#include "Resources.h"
#include "imgui.h"
#include "Camera.h"
#include "Entity.h"
#include "Scene.h"

BE::Camera* Application::camera = nullptr;
BE::Scene* Application::scene = nullptr;

void Application::OnEngineInit()
{
	Resources::Init();

	scene = BE::Scene::CreateScene("Graph");
	camera = new BE::Camera();

	auto entity = scene->GetEntityManager().CreateEntity("Camera");
	entity->AddComponent(camera);
	entity->transform.SetPosition(0, 0, 10);
	//this->texture = new BE::Texture(BE::File::LoadFile("C:/Users/Rhys/Desktop/circle.png"));

	graph = new GraphManager();
}

void Application::OnEngineDestroy()
{

}

void Application::OnEngineUpdate() 
{
	graph->Update();
}