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
#include "LightManager.h"
#include "Renderer.h"

#include "EventSystem.h"

#include "Chunk.h"

#include "BoundingBox.h"

#include "BuiltinShaders.h"


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

Application::Application(const BE::EngineDesc& engineDesc) : BE::BaseEngine(engineDesc)
{
	srand(time(NULL));

	this->CreateEngine(BE::RenderDesc::Default());

	BE::Gizmos::CreateGizmos();

	// Create Textures
	BE::Texture* texture = new BE::Texture(BE::File::LoadFile("data/textures/stone.png"));
	BE::Texture* texture_grass = new BE::Texture(BE::File::LoadFile("data/textures/grass.png"));
	BE::Texture* texture_ground = new BE::Texture(BE::File::LoadFile("data/textures/bricks.png"));
	BE::Texture* texture_sky = new BE::Texture(BE::File::LoadFile("data/textures/sky.png"));

	BE::TextureDesc desc = BE::TextureDesc();
	desc.format = BE::TextureFormat::RGB;
	desc.flags = BE::TextureFlags::INVERT_G;
	BE::Texture* texture_bricksNormal = new BE::Texture(BE::File::LoadFile("data/textures/stone_n.png"), desc); // brickwall_normal

	// Create materials
	BE::Material* mat = new BE::Material(BE_SHADER_STANDARD);
	mat->SetUniform("mainTexture", texture);
	mat->SetUniform("normTexture", texture_bricksNormal);
	mat->SetUniform("tiling", glm::vec2(1, 1));

	BE::Material* mat_sky = new BE::Material(BE_SHADER_UNLIT_TEXTURE);
	mat_sky->SetUniform("mainTexture", texture_sky);
	mat_sky->SetDepthWrite(false);

	BE::Material* mat_grass = new BE::Material(BE_SHADER_STANDARD);
	mat_grass->SetUniform("mainTexture", texture_grass);
	mat->SetUniform(BE_SHADER_UNI_ALPHACLIP, true);


	BE::Material* mat_ground = new BE::Material(BE::PremadeShaders::STANDARD);
	mat_ground->SetUniform("mainTexture", texture_ground);
	mat_ground->SetUniform("tiling", glm::vec2(15, 15));


	BE::Shader* texShader = BE::Shader::CreateShaderFromFile("Texture", "data/shaders/default/texture");
	BE::Material* mat_image = new BE::Material(texShader);


	// Create Main Scene
	mainScene = BE::Scene::CreateScene("Main");
	mainScene->GetLightManager().CreateShadowMap(1028 * 4, 1028 * 4);


	// Debug Camera
	auto entity_Cam = mainScene->GetEntityManager().CreateEntity("Camera", 0.0f, 0.0f, 10.0f);
	entity_Cam->AddComponent<BE::Camera>();
	entity_Cam->AddComponent<BE::DebugMovement>();
	entity_Cam->AddComponent<ExampleRaycast>();



	// Debug Plane
	auto entity_Plane = mainScene->GetEntityManager().CreateEntity("DebugPlane");
	entity_Plane->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_ground);
	entity_Plane->transform.SetPosition(0, -2.5f, 0);
	entity_Plane->transform.SetScale(50);

	
	//auto entity_image = mainScene->GetEntityManager().CreateEntity("Render Texture", 0, 0.5f, 5);
	//entity_image->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreatePlane(1), mat_image);
	//entity_image->transform.SetScale(3);
	//entity_image->transform.RotateX(90);

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
	entity_Sun->transform.SetPosition(3, 10, 5);
	entity_Sun->transform.RotateX(120.0f);
	//entity_Sun->transform.SetPosition(0, 10, 0);
	//entity_Sun->transform.RotateX(90.0f);



	// ORB
	//auto* sphereMeshRenderer = new BE::MeshRenderer(BE::MeshLoader::CreateSphere(1), mat_sky);
	//auto entity_orb = mainScene->GetEntityManager().CreateEntity("Orb");
	//entity_orb->AddComponent(sphereMeshRenderer);
	//entity_orb->AddComponent<ExampleComponent>();
	//entity_orb->transform.SetParent(entity_Cam);
	//sphereMeshRenderer->SetRenderPriority(BE_RENDERPRI_HIGHEST);
	//sphereMeshRenderer->SetShadowCasting(false);

	//entity_orb->AddComponent<ExampleComponent>();
	//sphereMeshRenderer->GetRenderLayer().ClearLayers();
	//sphereMeshRenderer->GetRenderLayer().AddLayer("Distort");
	//entity_orb->transform.SetPosition(0, 5, 0);

	// BOX
	//mainScene->GetEntityManager().CreatePrimitive("Sphere", BE::PrimitiveType::SPHERE_STATIC);

	auto entity_box1 = mainScene->GetEntityManager().CreateEntity("Box_1");
	//entity_box1->AddComponent<ExampleComponent>();
	entity_box1->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box1->transform.SetPosition(0, 1, 1);
	entity_box1->transform.SetScale(1);

	auto entity_box2 = mainScene->GetEntityManager().CreateEntity("Box_2_Child");
	entity_box2->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box2->transform.SetPosition(-5, 0, 2);
	entity_box2->transform.SetParent(entity_box1);

	auto entity_box3 = mainScene->GetEntityManager().CreateEntity("Box_3_Child");
	entity_box3->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box3->transform.SetPosition(1, 0, 5);
	entity_box3->transform.SetParent(entity_box1);

	auto entity_box4 = mainScene->GetEntityManager().CreateEntity("Box_4_Child");
	entity_box4->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box4->transform.SetPosition(4, 2, 3);
	entity_box4->transform.SetParent(entity_box1);
	entity_box4->AddComponent<ExampleComponent>();

	auto entity_box5 = mainScene->GetEntityManager().CreateEntity("Box_5_Child");
	entity_box5->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateCube(1), mat);
	entity_box5->transform.SetPosition(-1, 1, -3);
	entity_box5->transform.SetParent(entity_box2);


	///auto entity_circle = mainScene->GetEntityManager().CreateEntity("sphere");
	///entity_circle->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateSphere(1), mat);

	BE::TextureDesc terrainDesc = BE::TextureDesc();
	terrainDesc.filter = BE::Filtering::NEAREST;

	terrainTexture = new BE::Texture(2, 2, terrainDesc);
	BE::Material* terrainMaterial = new BE::Material(BE_SHADER_STANDARD);
	terrainMaterial->SetMainTexture(terrainTexture);

	// Terrain
	//auto terrain = mainScene->GetEntityManager().CreateEntity("Terrain");
	//terrain->transform.SetPosition(glm::vec3(0, 0, 0));
	//terrain->transform.RotateX(90);
	//terrain->transform.SetScale(glm::vec3(1, 1, 1));
	//terrain->AddComponent<BE::MeshRenderer>(BE::MeshLoader::CreateQuad(), terrainMaterial);
	//terrain->GetComponent<BE::MeshRenderer>()->SetMaterial(*);

	// Chunk
	//auto entity_chunk = mainScene->GetEntityManager().CreateEntity("Chunk");
	//entity_chunk->AddComponent<BE::MeshRenderer>(mat);
	//entity_chunk->AddComponent<Chunk>();

	BE::MeshData meshData = BE::MeshData();
	meshData.position = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	meshData.uvs = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	meshData.normals = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	auto meshDataEntity = mainScene->GetEntityManager().CreateEntity();

	BE::Material* meshDataEntityMat = new BE::Material(BE_SHADER_UNLIT_TEXTURE);

	meshDataEntity->AddComponent<BE::MeshRenderer>(meshData, meshDataEntityMat);
	//meshDataEntity->transform.RotateX(90);
	meshDataEntity->transform.SetPosition(glm::vec3(-10, 5, 0));
	meshDataEntity->transform.SetScale(3);


	BE::Shader* wormShader = BE::Shader::CreateShader("worm", BE::File::LoadFile("data/shaders/default/standardBone.vert").c_str(), BE::ShaderSource::FRAG_STANDARD);
	wormMaterial = new BE::Material(wormShader);

	auto worm = mainScene->GetEntityManager().CreateEntity("Worm");
	auto wormRenderer = worm->AddComponent<BE::MeshRenderer>(BE::MeshLoader::LoadFile("C:/Users/Rhys/Desktop/Worm2.dae"), wormMaterial); // worm->AddComponent<BE::MeshRenderer>(BE::MeshLoader::LoadFile(R"(C:\Users\Rhys\Desktop\Worm2.fbx)"), wormMaterial);
	worm->transform.SetPosition(0, 2, 0);
}

Application::~Application()
{
	// TODO: Clean up

	delete mainScene;
}

void Application::OnFrameUpdate()
{
	static float fps = 0.0f;
	static float ms = 0.0f;
	static float lastUpdate = 1.0f;

	if (lastUpdate > 0.1f) {
		fps = 1.0f / BE::Time::DeltaTime();
		ms = BE::Time::DeltaTime();
		lastUpdate = 0.0f;
	}

	lastUpdate += BE::Time::DeltaTime();


	auto wormRenderer = mainScene->GetEntityManager().GetEntity("Worm")->GetComponent<BE::MeshRenderer>();
	
	glm::mat4 m1 = wormRenderer->GetMesh()->GetMeshData()->boneOffsets[0];
	glm::mat4 m2 = wormRenderer->GetMesh()->GetMeshData()->boneOffsets[1];
	
	static glm::vec3 pos2 = glm::vec3(0, 0, 0);
	
	ImGui::Begin("Bones");
	ImGui::DragFloat3("m1", &pos2[0], 0.1f);
	ImGui::End();
	
	m2 = glm::translate(m1, pos2);
	
	wormMaterial->SetUniform("bone1", m1);
	wormMaterial->SetUniform("bone2", m2);



	BE::Gizmos::DrawCube(1.0f / 2.0f, glm::vec3(0.5, 0, 0));

	ImGui::Begin("Worms Terrain");

	if (ImGui::Button("Generate Terrain")) {
		std::vector<unsigned char> pixels = std::vector<unsigned char>();
		glm::vec2 size = terrainTexture->GetSize();
		int channels = terrainTexture->GetChannels();

		for (int y = 0; y < size.y; y++) {
			char value = y > (size.y / 2.0f) ? 255 : 0;
			
			for (int x = 0; x < size.x; x++) {

				pixels.push_back(value);
				if (channels >= 2)
					pixels.push_back(value);
				if (channels >= 3)
					pixels.push_back(value);
				if (channels >= 4)
					pixels.push_back(255);
			}
		}

		terrainTexture->SetPixels(pixels);
	}

	ImGui::End();







	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.2f", fps);
	ImGui::Text("MS: %.6f", ms);
	ImGui::Image((void*)mainScene->GetLightManager().GetShadowMapFrameBuffer()->GetDepthTexture()->GetID(), ImVec2(512, 512), ImVec2(1, 0), ImVec2(0, 1));
	ImGui::End();
}
