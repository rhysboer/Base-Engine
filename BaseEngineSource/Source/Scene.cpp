#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "EntityManager.h"
#include "LightManager.h"
#include "IRender.h"
#include <assert.h>

#if _DEBUG
#include "Gizmos.h"
#endif

namespace BE {
	std::unordered_map<std::string, Scene*> Scene::scenes = std::unordered_map<std::string, Scene*>();

	Scene* Scene::CreateScene(const char* name) {
		assert(scenes.find(name) == scenes.end());
		Scene* scene = new Scene(name);

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
		lightManager = new LightManager(this);
	}

	void Scene::OnUpdate() {
		entityManager->Update();
	}

    void Scene::UpdateScenes() {
		for(auto& it : scenes) {
			if(!it.second->isEnabled)
				continue;

			it.second->OnUpdate();
		}
    }

    //void Scene::DrawScenes() {
	//	for(auto& it : scenes) {
	//		if(!it.second->isEnabled)
	//			continue;
	//
	//		it.second->OnRender();
	//	}
	//}
}