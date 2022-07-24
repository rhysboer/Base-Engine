#include "Application.h"
#include "Gizmos.h"
#include "TimeManager.h"

#include "Entity.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"
#include "ShaderManager.h"

#include "Material.h"
#include "Texture.h"
#include "Input.h"
#include "PlaneCollider.h"

#include "Camera.h"
#include "DebugMovement.h"
#include "ExampleComponent.h"
#include "ExampleRaycast.h"

#include "Mesh.h"
#include "File.h"

#include "Light.h"

#include "Common.h"
#include "Logging.h"

#include "imgui.h"

#include <iostream>
#include "EntityDebugger.h"

#include "DebugComponent.h"
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

	//debugger = new BE::Debugger();

	//BE::Gizmos::Init();

	//BE::TextureManager::LoadTexture("mainTexture", R"(C:\Users\Rhys\Desktop\New folder\bunny-atlas.jpg)");//"data/textures/default.png");
	//ray = camera->CreateRayFromMouse();
	BE::Texture* texture = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\m10_stair_bridge_L.png)"));
	BE::Texture* texture2 = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\m10_01_tree_01.png)"));
	BE::Texture* texture_plant = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\m10_grass_03.png)"));
	BE::Texture* texture_window = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\blending_transparent_window.png)"));

	BE::Texture* texture_bricks = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\brickwall.jpg)"));
	BE::Texture* texture_bricksNormal = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\brickwall_normal.jpg)")); // brickwall_normal

	BE::Texture* texture_orb = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dunrik_Pfp.png)"));

	BE::Material* mat = new BE::Material(BE_SHADER_STANDARD);
	mat->SetUniform("mainTexture", texture);
	
	BE::Material* mat2 = new BE::Material(BE_SHADER_STANDARD);
	//mat2->SetUniform("mainTexture", texture2);

	BE::Material* mat_window = new BE::Material(BE_SHADER_STANDARD);
	mat_window->SetUniform("mainTexture", texture_window);
	mat_window->SetUniform(BE_SHADER_UNI_TRANSPARENT, true);
	mat_window->SetUniform(BE_SHADER_UNI_ALPHACLIP, true);
	mat_window->SetUniform(BE_SHADER_UNI_ALPHACLIP_THRES, 0.1f);
	mat_window->SetTransparent(true);

	BE::Material* material_plant = new BE::Material(BE_SHADER_STANDARD);
	material_plant->SetUniform("mainTexture", texture_plant);
	material_plant->SetUniform(BE_SHADER_UNI_ALPHACLIP, true);
	material_plant->SetUniform(BE_SHADER_UNI_ALPHACLIP_THRES, 0.9f);
	
	BE::Material* material_orb = new BE::Material(BE_SHADER_STANDARD);
	material_orb->SetUniform("mainTexture", texture_orb);

	BE::Material* material_bricks = new BE::Material(BE_SHADER_STANDARD);
	material_bricks->SetUniform("mainTexture", texture_bricks);
	material_bricks->SetUniform("normTexture", texture_bricksNormal);
	material_bricks->SetUniform("tiling", glm::vec2(5, 5));

	mainScene = BE::Scene::CreateScene("Main");
	

	//BE::Mesh* flverMesh = BE::MeshLoader::LoadFile("C:/Users/Rhys/Desktop/m2022B2A10.flver");

	// Dark souls stuff
	//std::string PATH = R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\)";
	//BE::Texture* texture_ds_m10_base_wall_01 = new BE::Texture(BE::File::LoadFile(PATH + "m10_base_floor_ground_02.png"));
	//BE::Texture* texture_ds_m10_base_wall_n_01 = new BE::Texture(BE::File::LoadFile(PATH + "m10_base_floor_ground_02_n.png"), BE::TextureFlags::INVERT_G);
	//BE::Texture* texture_ds_m10_cliff_01 = new BE::Texture(BE::File::LoadFile(PATH + "m10_cliff_01.png"));
	//BE::Texture* texture_ds_m10_cliff_n_01 = new BE::Texture(BE::File::LoadFile(PATH + "m10_cliff_01_n.png"), BE::TextureFlags::INVERT_G);
	//BE::Texture* texture_ds_m10_base_stair = new BE::Texture(BE::File::LoadFile(PATH + "m10_base_stair.png"));
	//BE::Texture* texture_ds_m10_base_stair_n = new BE::Texture(BE::File::LoadFile(PATH + "m10_base_stair_n.png"), BE::TextureFlags::INVERT_G);


	//BE::Material* material_m10_base_wall_01 = new BE::Material(BE_SHADER_STANDARD);
	//material_m10_base_wall_01->SetUniform("mainTexture", texture_ds_m10_base_wall_01);
	//material_m10_base_wall_01->SetUniform("normTexture", texture_ds_m10_base_wall_n_01);
	//
	//BE::Material* material_m10_cliff_01 = new BE::Material(BE_SHADER_STANDARD);
	//material_m10_cliff_01->SetUniform("mainTexture", texture_ds_m10_cliff_01);
	//material_m10_cliff_01->SetUniform("normTexture", texture_ds_m10_cliff_n_01);
	//
	//BE::Material* material_m10_base_stair = new BE::Material(BE_SHADER_STANDARD);
	//material_m10_base_stair->SetUniform("mainTexture", texture_ds_m10_base_stair);
	//material_m10_base_stair->SetUniform("normTexture", texture_ds_m10_base_stair_n);



	/*
	std::vector<std::string> names = {
		"m0000B2A10.obj",
		//"m0030B2A10.obj",
		//"m0100B2A10.obj",
		//"m0101B2A10.obj",
		//"m2000B2A10.obj",
		//"m2001B2A10.obj",
		//"m2002B2A10.obj",
		//"m2010B2A10.obj",
		//"m2020B2A10.obj",
		//"m2021B2A10.obj",
		//"m2022B2A10.obj",
		//"m2023B2A10.obj",
		//"m2030B2A10.obj",
		//"m2040B2A10.obj",
		//"m2110B2A10.obj",
		//"m2120B2A10.obj",
		//"m2130B2A10.obj",
		//"m2200B2A10.obj",
		//"m2210B2A10.obj",
		//"m2300B2A10.obj",
		//"m2301B2A10.obj",
		//"m2310B2A10.obj",
		//"m2901B2A10.obj",
		//"m4000B2A10.obj",
		//"m4001B2A10.obj",
		//"m4002B2A10.obj",
		//"m4010B2A10.obj",
		//"m4011B2A10.obj",
		//"m4012B2A10.obj",
		//"m4013B2A10.obj",
		//"m4100B2A10.obj",
		//"m4110B2A10.obj",
		//"m5000B2A10.obj",
		//"m5001B2A10.obj",
		//"m5002B2A10.obj",
		//"m5003B2A10.obj",
		//"m5004B2A10.obj",
		//"m5005B2A10.obj",
		//"m5006B2A10.obj",
		//"m5007B2A10.obj",
		//"m5008B2A10.obj",
		//"m5009B2A10.obj",
		//"m5010B2A10.obj",
		//"m5011B2A10.obj",
		//"m5012B2A10.obj",
		//"m5013B2A10.obj",
		//"m5014B2A10.obj",
		//"m5015B2A10.obj",
		//"m5016B2A10.obj",
		//"m5017B2A10.obj",
		//"m5018B2A10.obj",
		//"m5019B2A10.obj",
		//"m5020B2A10.obj",
		//"m5030B2A10.obj",
		//"m5040B2A10.obj",
		//"m5050B2A10.obj",
		//"m5060B2A10.obj",
		//"m5061B2A10.obj",
		//"m5062B2A10.obj",
		//"m5063B2A10.obj",
		//"m5064B2A10.obj",
		//"m6000B2A10.obj",
		//"m6100B2A10.obj",
		//"m7000B2A10.obj",
		//"m7010B2A10.obj",
		//"m7011B2A10.obj",
		//"m8000B2A10.obj",
		//"m8100B2A10.obj",
		//"m8101B2A10.obj",
		//"m8200B2A10.obj",
		//"m8500B2A10.obj",
		//"m9000B2A10.obj",
		//"m9001B2A10.obj"
	};
	
	std::vector<std::vector<BE::Material*>> materials = std::vector<std::vector<BE::Material*>>({
		std::vector<BE::Material*>({ material_m10_base_wall_01, material_m10_cliff_01, material_m10_cliff_01, material_m10_base_stair }),
	});

	//
	for (int i = 0; i < names.size(); i++) {
		BE::Mesh* mesh = BE::MeshLoader::LoadFile(
			("C:\\Users\\Rhys\\Desktop\\Dark Souls Assets\\" + names[i]).c_str()
		);
		mesh->Apply();
	
		auto newEnt = mainScene->GetEntityManager().CreateEntity();
		newEnt->AddComponent<BE::MeshRenderer>(mesh, materials[i]);
	}
	*/

	
	//BE::Mesh* mesh = new BE::Mesh(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\m8500B2A10.obj)"));//BE::File::LoadFile(R"(C:\Users\Rhys\source\repos\BaseEngineSource\BaseEngineSource\data\bunny.obj)"));
	//mesh->Apply();
	//glm::mat4 transform = glm::mat4(1);
	//for(int i = 0; i < 2; i++)
	//	transform = glm::rotate(transform, glm::radians(-90.0f * 1), { 0, 1, 0 });
	//
	//mesh->TransformMesh(transform, { 0, 1 });
	//mesh->UpdateMesh();


	// Debug Camera
	auto entity_Cam = mainScene->GetEntityManager().CreateEntity("Camera", 0.0f, 0.0f, 10.0f);
	entity_Cam->AddComponent<BE::Camera>();
	entity_Cam->AddComponent<BE::DebugMovement>();
	entity_Cam->AddComponent<ExampleRaycast>();

	// Debug Plane
	auto entity_Plane = mainScene->GetEntityManager().CreateEntity();
	entity_Plane->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat);
	entity_Plane->transform.SetPosition(0, -1.5, 0);
	entity_Plane->transform.SetScale(5);
	//auto collider = entity_Plane->AddComponent<BE::PlaneCollider>(1);
	

	//auto entity_box = mainScene->GetEntityManager().CreateEntity();
	//entity_box->AddComponent<BE::MeshRenderer>(mesh, mat2);

	// Window
	//auto entity_window1 = mainScene->GetEntityManager().CreateEntity("Window 1");
	//entity_window1->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_window);
	//entity_window1->transform.RotateX(90);
	//entity_window1->transform.SetPosition(0, -0.5f, 2);

	// Normal Map Plane
	//auto normal_plane_mesh = BE::MeshLoader::LoadFile(R"(C:\Users\Rhys\Desktop\Work\Plane.obj)"); // , (int)BE::MeshLoader::MeshFlags::GENERATE_TANGENT_SPACE
	//normal_plane_mesh->Apply();
	//
	//auto entity_normal = mainScene->GetEntityManager().CreateEntity("Normal Test");
	//entity_normal->AddComponent<BE::MeshRenderer>(normal_plane_mesh, material_bricks);
	//entity_normal->transform.SetPosition(-10, 0, 0);
	//entity_normal->transform.RotateX(90);
	//entity_normal->transform.SetScale(5);
	


	// Plant
	//auto entity_plant = mainScene->GetEntityManager().CreateEntity("Plant");
	//entity_plant->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), material_plant);
	//entity_plant->transform.RotateX(90);
	//entity_plant->transform.SetPosition(0, -0.5f, 0);

	// Window
	//auto entity_window = mainScene->GetEntityManager().CreateEntity("Window 2");
	//entity_window->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_window);
	//entity_window->transform.RotateX(90);
	//entity_window->transform.SetPosition(0, -0.5f, 1);


	
	// SUN
	auto entity_Sun = mainScene->GetEntityManager().CreateEntity("Sun");
	//entity_Sun->AddComponent<BE::DebugComponent>();
	entity_Sun->AddComponent<BE::Light>(BE::LightType::SPOT);
	entity_Sun->transform.RotateX(90.0f);
	entity_Sun->transform.SetPosition(0, 1, 0.5f);




	// ORB
	auto* sphereMeshRenderer = new BE::MeshRenderer(BE::MeshLoader::CreateSphere(1), material_orb);
	auto entity_orb = mainScene->GetEntityManager().CreateEntity("Orb");
	entity_orb->AddComponent(sphereMeshRenderer);
	entity_orb->AddComponent<ExampleComponent>();
	

	std::cout << BE::IComponent::GetComponentID<BE::IRender>() << std::endl;
	std::cout << BE::IComponent::GetComponentID<BE::MeshRenderer>() << std::endl;
	
}

void Application::OnEngineDestroy() {
}

void Application::OnEngineUpdate() {
	BE::Debug::EntityDebugger::Update();

	auto* camera = BE::Scene::GetScene("Main")->GetEntityManager().GetEntity("Camera")->GetComponent<BE::Camera>();

	static glm::vec3 position = glm::vec3(0);
	ImGui::Begin("Test");
	ImGui::DragFloat3("Pos", &position[0]);

	BE::Gizmos::DrawCube(0.5f, position, COLOUR_RED);

	glm::vec2 screenPos = camera->WorldSpaceToScreenSpace(position);
	ImGui::Text("%f, %f", screenPos.x, screenPos.y);
	ImGui::End();

	//std::vector<float> vertexData = std::vector<float>();
	//std::vector<unsigned int> indices = std::vector<unsigned int>();
	
	//glm::vec3 pos = glm::vec3(0, 1, 0);
	//glm::vec3 norm = glm::vec3(0);
	//glm::vec2 tex = glm::vec2(0);
	//
	//const float BE_PI = 3.1415926f;
	//const float radius = 1.0f;
	//
	//const float sector = 32;
	//const float stack = 29;
	//
	//const float sectorStep = (2.0f * BE_PI) / sector;
	//const float stackStep = BE_PI / stack;
	//
	////float angle = 
	//
	//// Top
	////AddData();
	//BE::Gizmos::DrawCube(0.1f, glm::vec3(0, 1, 0), COLOUR_RED);
	//
	//// Stack
	//for (int i = 1; i < stack; i++) {
	//	float stackAngle = BE_PI / 2.0f - i * stackStep;
	//
	//	// Sector
	//	for (int j = 0; j <= sector; j++) {
	//		float sectorAngle = j * sectorStep; //(2.0f * BE_PI) * ((sectorStep * j) / sector);
	//
	//		pos.x = (radius * glm::cos(stackAngle)) * glm::cos(sectorAngle);
	//		pos.y = radius * glm::sin(stackAngle);
	//		pos.z = (radius * glm::cos(stackAngle)) * glm::sin(sectorAngle);
	//
	//		BE::Gizmos::DrawCube(0.05f, pos, pos + 1.0f / 2.0f);
	//		//AddData();
	//	}
	//}
	//
	//BE::Gizmos::DrawCube(0.1f, glm::vec3(0, -1, 0), COLOUR_RED);
	// Bottom
	//pos.x = 0;
	//pos.y = -1;
	//pos.z = 0;
	//AddData();



	//BE::Gizmos::DrawLine({ 0,0,0 }, a1, {}, COLOUR_BLUE);
	//BE::Gizmos::DrawLine({ 0,0,0 }, a2, {}, COLOUR_RED);
	//BE::Gizmos::DrawLine({ 0,0,0 }, a3, {}, COLOUR_GREEN);;

	//if (BE::Input::IsKeyPressed(GLFW_KEY_T)) {
	//	mainScene->GetEntityManager().GetEntity("ASS")->GetComponent<BE::Mesh>()->TransformMesh()
	//}

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
