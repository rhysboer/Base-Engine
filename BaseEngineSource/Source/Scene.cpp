#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "EntityManager.h"
#include "LightManager.h"
#include <assert.h>

#if _DEBUG
#include "Gizmos.h"
#endif

namespace BE {
	Scene* Scene::activeScene = nullptr;
	std::unordered_map<std::string, Scene*> Scene::scenes = std::unordered_map<std::string, Scene*>();

	Scene::~Scene()
	{
		scenes.erase(name);
		
		if (activeScene == this)
			activeScene = nullptr;

		delete entityManager;
		//delete lightManager;
	}

	Scene* Scene::CreateScene(const char* name) {
		BE_ASSERT(scenes.find(name) == scenes.end(), "Duplicated name");
		Scene* scene = new Scene(name);

		if (activeScene == nullptr)
			activeScene = scene;

		scenes.emplace(name, scene);
		return scene;
	}

	Scene* Scene::GetScene(const char* name)
	{
		auto iter = scenes.find(name);
		if (iter == scenes.end())
			return nullptr;
		return (*iter).second;
	}

	Scene::Scene(const char* name)
		: name(name), isEnabled(true)
	{ 
		entityManager = new EntityManager(this);

		// TODO: Lightmanager shouldn't be instantiated per scene 
		lightManager = new LightManager(this);
		//sceneRenderer = &renderer;
	}

	void Scene::UpdateActiveScene()
	{
		if (activeScene != nullptr) 
		{
			activeScene->GetEntityManager().Update();
		}
	}

	void Scene::PhysicsStepActiveScene(const float& dt)
	{
		if (activeScene != nullptr)
		{
			activeScene->GetEntityManager().PhysicsStep(dt);
		}
	}
}