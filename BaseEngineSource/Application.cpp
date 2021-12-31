#include "Application.h"
#include "Gizmos.h"
#include "TimeManager.h"

#include "Entity.h"
#include "MeshRenderer.h"
#include "MeshUtil.h"
#include "ShaderManager.h"

#include "Material.h"
#include "TextureManager.h"
#include "Input.h"
#include "PlaneCollider.h"
#include "Debugger.h"

#include "Camera.h"
#include "DebugMovement.h"
#include "ExampleComponent.h"
#include "ExampleRaycast.h"

#include <iostream>

/*
	--- TODO ---
	- Add Scenes
		contain all entities registered to it
		have a render function
		should only render with cameras active in their scene? 

	- Subtype Components
		Subtype components are components that act the same way as other components but need different data, for example lights.
		There is many different type of lights, directional, spot, area.
		We shouldnt need a system for each individual light, they all should fall under 1 system.
		So within the lighting system/manager, we can register any new lighting component. 
		This will also work for physical objects, rendering and many others.


*/


void Application::OnEngineInit() {
	//camera = new BE::FlyCamera(glm::vec3(0, 0, 10)); //BE::Camera::CreateCamera(glm::vec3(0, 5, 10), BE::Camera::CameraType::PERSPECTIVE, 60.0f);
	//camera->LookAt(glm::vec3(0));

	debugger = new BE::Debugger();

	BE::Gizmos::Init();

	int id = BE::ShaderManager::AddShaderFile("ass", "data/shaders/default/standard");
	BE::TextureManager::LoadTexture("mainTexture", "data/textures/default.png");

	//ray = camera->CreateRayFromMouse();

	BE::Material* mat = new BE::Material(id);
	mat->SetUniform("colour", glm::vec3(1, 1, 1));
	

	mainScene = BE::Scene::CreateScene("Main");

	// Debug Camera
	auto entity_Cam = mainScene->GetEntityManager().CreateEntity({0.0f, 0.0f, 10.0f}, "Camera");
	entity_Cam->AddComponent<BE::Camera>();
	entity_Cam->AddComponent<BE::DebugMovement>();
	entity_Cam->AddComponent<ExampleRaycast>();
	
	// Debug Cube
	auto entity_Cube = mainScene->GetEntityManager().CreateEntity();
	entity_Cube->AddComponent<BE::MeshRenderer>(BE::MeshUtil::CreateCube(), mat);
	entity_Cube->AddComponent<ExampleComponent>();

	// Debug Plane
	auto entity_Plane = mainScene->GetEntityManager().CreateEntity();
	entity_Plane->AddComponent<BE::MeshRenderer>(BE::MeshUtil::CreatePlane(5), mat);
	entity_Plane->AddComponent<BE::PlaneCollider>(5.0f);
	//entity_Plane->AddComponent<ExampleComponent>();
	//entity_Plane->transform.RotateX(90.0f);
	entity_Plane->transform.SetPosition(0, -1, 0);

	glm::quat quat = glm::rotate(glm::quat(1.0f, 0, 0, 0), glm::radians(90.0f), glm::vec3(0, 0, 1));
	glm::mat4 mat4 = glm::mat4_cast(glm::quat(0.707106f, 0, 0, 0.707106f));
	//quat = glm::quat_cast(mat4);

	std::cout << BE::IComponent::GetComponentID<BE::IRender>() << std::endl;
	std::cout << BE::IComponent::GetComponentID<BE::MeshRenderer>() << std::endl;

	//BE::Entity* entity = new BE::Entity();
	//entity->AddComponent<BE::MeshRenderer>(BE::MeshUtil::CreateCube(), mat);
	//entity->transform.Translate(0, 2, 0);
	
	//BE::Entity* grid = new BE::Entity();
	//grid->AddComponent<BE::MeshRenderer>(BE::MeshUtil::CreatePlane(10), mat);
	//grid->AddComponent<BE::PlaneCollider>(glm::vec2(10.0f));
}

void Application::OnEngineDestroy() {
}

void Application::OnEngineUpdate() {
	debugger->Update();

	//camera->Update();

	//glm::mat4 mat = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 2, 0)), BE::Time::TotalTime(), glm::vec3(0.3, 1, 1.2));
	
	//BE::Gizmos::DrawGrid(20, 40, glm::mat4(1), COLOUR_GREEN);
	//BE::Gizmos::DrawCube(1);

	//if(BE::Input::IsKeyDown(GLFW_KEY_R)) {
	//	ray = camera->CreateRayFromMouse();
	//}

	//BE::Gizmos::DrawRay(ray, COLOUR_MAGENTA);
	//glm::vec3 pos = camera->ScreenSpaceToWorldSpace(BE::Input::MousePosition());
	//BE::Gizmos::DrawCube(0.02f, pos);
}

void Application::OnEngineRender() {
	//BE::Gizmos::DrawCube(glm::vec3(0), 1);

	//BE::Gizmos::Render(camera->GetProjectionView());
	//BE::PhysicsManager::RenderDebug();
}
