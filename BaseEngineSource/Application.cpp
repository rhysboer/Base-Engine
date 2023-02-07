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

#include "BEMath.h"
#include "BEGlobal.h"

#include "Mesh.h"
#include "File.h"

#include "Light.h"

#include "Common.h"
#include "Logging.h"

#include "imgui.h"

#include <iostream>
#include "EntityDebugger.h"

#include "Renderer.h"

#include "EventSystem.h"

#include "Chunk.h"

#include "BoundingBox.h"


/*
void Application::OnEngineInit()
{

}

void Application::OnEngineDestroy() {
}

void Application::OnEngineUpdate() {
	//BE::Debug::EntityDebugger::Update();
	////BE::Gizmos::DrawThickLine(glm::vec3(0), glm::vec3(15, 15, 0), 0.5f);
	//
	//static float fps = 0.0f;
	//static float ms = 0.0f;
	//static float lastUpdate = 1.0f;
	//
	//if (lastUpdate > 0.1f) {
	//	fps = 1.0f / BE::Time::DeltaTime();
	//	ms = BE::Time::DeltaTime();
	//	lastUpdate = 0.0f;
	//}
	//
	//lastUpdate += BE::Time::DeltaTime();
	//
	//
	//ImGui::Begin("Performance");
	//ImGui::Text("FPS: %.2f", fps);
	//ImGui::Text("MS: %.6f", ms);
	//ImGui::End();
}
*/

Application::Application()
{
	srand(time(NULL));
	
	auto engineDesc = BE::EngineDesc();
	engineDesc.name = "Engine Title";
	engine = BE::BaseEngine::CreateEngine(engineDesc);

	BE::EventSystem::EventSubscribe(BE_EVENT_ENGINE_UPDATE, BE_EVENT_FUNC_CREATE(OnUpdate));

	//debugger = new BE::Debugger();

	BE::Gizmos::CreateGizmos();

	//BE::TextureManager::LoadTexture("mainTexture", R"(C:\Users\Rhys\Desktop\New folder\bunny-atlas.jpg)");//"data/textures/default.png");
	//ray = camera->CreateRayFromMouse();
	BE::Texture* texture = new BE::Texture(BE::File::LoadFile(R"(data\textures\stone.png)"));
	BE::Texture* texture_grass = new BE::Texture(BE::File::LoadFile(R"(data\textures\grass.png)"));
	BE::Texture* texture_ground = new BE::Texture(BE::File::LoadFile("data/textures/bricks.png"));
	//BE::Texture* texture2 = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\m10_01_tree_01.png)"));
	//BE::Texture* texture_plant = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\Dark Souls Assets\textures\m10_grass_03.png)"));
	//BE::Texture* texture_window = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\blending_transparent_window.png)"));
	//
	//BE::Texture* texture_bricks = new BE::Texture(BE::File::LoadFile(R"(C:\Users\Rhys\Desktop\brickwall.jpg)"));
	BE::TextureDesc desc = BE::TextureDesc();
	desc.format = BE::TextureFormat::RGB;
	desc.flags = BE::TextureFlags::INVERT_G;

	BE::Texture* texture_bricksNormal = new BE::Texture(BE::File::LoadFile(R"(data\textures\stone_n.png)"), desc); // brickwall_normal



	BE::Material* mat = new BE::Material(BE_SHADER_STANDARD);
	mat->SetUniform("mainTexture", texture);
	mat->SetUniform("normTexture", texture_bricksNormal);
	mat->SetUniform("tiling", glm::vec2(1, 1));

	BE::Material* mat_sky = new BE::Material(BE_SHADER_STANDARD);
	mat_sky->SetUniform("mainTexture", texture);
	mat_sky->SetUniform("normTexture", texture_bricksNormal);
	mat_sky->SetUniform("tiling", glm::vec2(1, 1));

	BE::Material* mat_grass = new BE::Material(BE_SHADER_STANDARD);
	mat_grass->SetUniform("mainTexture", texture_grass);
	//mat->SetUniform(BE_SHADER_UNI_ALPHACLIP, true);
	mat_grass->SetUniform(BE_SHADER_UNI_TRANSPARENT, true);
	mat_grass->SetTransparent(true);


	BE::Material* mat_ground = new BE::Material(BE_SHADER_STANDARD);
	mat_ground->SetUniform("mainTexture", texture_ground);
	mat_ground->SetUniform("tiling", glm::vec2(15, 15));


	BE::Texture* distortNormal = new BE::Texture(BE::File::LoadFile(R"(C:/Users/Rhys/Desktop/2999-normal.jpg)"));
	BE::Texture* screenGrab = new BE::Texture(1280, 720);


	BE::ShaderManager::AddShaderFile("Sphere", "data/shaders/default/sphere");
	//BE::Material* material_orb = new BE::Material("Sphere");
	//material_orb->SetUniform("mainTexture", screenGrab);
	//material_orb->SetUniform("distortTexture", distortNormal);


	//BE::RenderPass* pass = new BE::RenderPass();
	//pass->name = "Distort";
	//pass->data = screenGrab;
	//pass->renderLayers.emplace(BE::RenderLayer::CreateLayer("Distort"));
	//pass->renderFunc = [](const BE::PassInfo& pass, std::vector<BE::IRender*>& renderables) {
	//	BE::Texture* texture = (BE::Texture*)pass.passDesc->data;
	//	BE::BaseEngine::GetRenderer()->GetFrameBuffer()->CopyColorTo(texture);
	//
	//	for (int i = 0; i < renderables.size(); i++) {
	//		if (renderables[i]->GetMaterial(0)->IsTransparent())
	//			continue;
	//
	//		renderables[i]->OnRender();
	//	}
	//};

	engine->GetRenderer()->CreateRenderer();
	//this->GetRenderer()->AddRenderPass(pass);

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
	auto entity_Plane = mainScene->GetEntityManager().CreateEntity("DebugPlane");
	entity_Plane->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_ground);
	entity_Plane->transform.SetPosition(0, -1.5f, 0);
	entity_Plane->transform.SetScale(50);


	//for (int i = 0; i < 20; i++) {
	//	float x = BE::Math::RandomFloat(-2.5f, 2.5f);
	//	float z = BE::Math::RandomFloat(-2.5f, 2.5f);
	//	float r = BE::Math::RandomFloat(0.0f, 180.0f);
	//
	//	auto grass_Plane = mainScene->GetEntityManager().CreateEntity();
	//	grass_Plane->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_grass);
	//	grass_Plane->transform.SetPosition(glm::vec3(x, 0, z));
	//	grass_Plane->transform.RotateY(r);
	//	grass_Plane->transform.RotateX(90.0f);
	//
	//	auto grass_Plane2 = mainScene->GetEntityManager().CreateEntity();
	//	grass_Plane2->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_grass);
	//	grass_Plane2->transform.SetPosition(glm::vec3(x, 0, z));
	//	grass_Plane2->transform.RotateY(r + 90.0f);
	//	grass_Plane2->transform.RotateX(90.0f);
	//}

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
	entity_Sun->AddComponent<BE::Light>(BE::LightType::DIRECTION);
	entity_Sun->transform.RotateX(90.0f);
	entity_Sun->transform.SetPosition(0, 1, 0);



	// ORB
	auto* sphereMeshRenderer = new BE::MeshRenderer(BE::MeshLoader::CreateSphere(1), mat_sky);
	auto entity_orb = mainScene->GetEntityManager().CreateEntity("Orb");
	entity_orb->AddComponent(sphereMeshRenderer);
	entity_orb->AddComponent<ExampleComponent>();
	//entity_orb->AddComponent<ExampleComponent>();
	//sphereMeshRenderer->GetRenderLayer().ClearLayers();
	//sphereMeshRenderer->GetRenderLayer().AddLayer("Distort");
	//entity_orb->transform.SetPosition(0, 5, 0);

	// BOX
	auto entity_box1 = mainScene->GetEntityManager().CreateEntity("Box_1");
	entity_box1->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box1->transform.SetPosition(0, 1, 1);

	// Chunk
	//auto entity_chunk = mainScene->GetEntityManager().CreateEntity("Chunk");
	//entity_chunk->AddComponent<BE::MeshRenderer>(mat);
	//entity_chunk->AddComponent<Chunk>();


	// Tree
	//auto entity_tree = mainScene->GetEntityManager().CreateEntity("Tree");
	//entity_tree->AddComponent<BE::MeshRenderer>(BE::MeshLoader::LoadFile(R"(C:\Users\Rhys\Desktop\assets\Trees\M_tree_mid_2.obj)"), mat); // { mat_bark, mat_bark, mat_branch1, mat_branch2, mat_leaves }
	//entity_tree->transform.SetScale(0.01f);
}

Application::~Application()
{
	delete engine;
}

void Application::Play()
{
	engine->Run();
}

void Application::OnUpdate(const void* const data)
{
	BE::Debug::EntityDebugger::Update();
	//BE::Gizmos::DrawThickLine(glm::vec3(0), glm::vec3(15, 15, 0), 0.5f);
	
	static float fps = 0.0f;
	static float ms = 0.0f;
	static float lastUpdate = 1.0f;
	
	if (lastUpdate > 0.1f) {
		fps = 1.0f / BE::Time::DeltaTime();
		ms = BE::Time::DeltaTime();
		lastUpdate = 0.0f;
	}
	
	lastUpdate += BE::Time::DeltaTime();
	
	
	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.2f", fps);
	ImGui::Text("MS: %.6f", ms);
	if (ImGui::Button("Stop Engine :)")) {
		auto entity_box1 = mainScene->GetEntityManager().CreateEntity("Box_");
		entity_box1->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1));
		entity_box1->transform.SetPosition(0, 5, 1);

		//BE::BaseEngine::GetEngine()->Stop();
	}
	ImGui::End();
}
