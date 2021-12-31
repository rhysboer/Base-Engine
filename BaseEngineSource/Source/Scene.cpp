#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "EntityManager.h"
#include <assert.h>
#include "IRender.h"
#include "UniformBuffer.h"

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

	Scene::Scene(const char* name)
		: name(name), isEnabled(true)
	{ 
		entityManager = new EntityManager(this);
	}

	void Scene::OnUpdate() {
		entityManager->Update();
	}

	void Scene::OnRender() {
		ComponentArray* const cameras = entityManager->GetComponents<Camera>();
		ComponentArray* const components = entityManager->GetComponents<IRender>();

		if(cameras == nullptr || components == nullptr)
			return;

		for(int i = 0; i < cameras->Size(); i++) {
			Camera* camera = (Camera*)cameras->AtIndex(i);

			UniformBuffer::GetUniformBuffer("Camera")->SetValue(camera->GetProjectionView());


			for(int j = 0; j < components->Size(); j++) {
				IRender* renderer = (IRender*)components->AtIndex(j);
				
				if(!renderer->IsActive())
					continue;
				
				renderer->OnRender(camera);
			}

#if _DEBUG
			BE::Gizmos::Render(camera->GetProjectionView());
#endif
		}
	}





    void Scene::UpdateScenes() {
		for(auto& it : scenes) {
			if(!it.second->isEnabled)
				continue;

			it.second->OnUpdate();
		}
    }

    void Scene::DrawScenes() {
		for(auto& it : scenes) {
			if(!it.second->isEnabled)
				continue;

			it.second->OnRender();
		}
	}
}