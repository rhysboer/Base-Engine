#include "EntityManager.h"
#include "Entity.h"
#include <assert.h>

namespace BE {
	EntityManager::EntityManager(Scene* scene) : scene(scene) {
	}

	EntityManager::~EntityManager() {
	}

	Entity* const EntityManager::CreateEntity(glm::vec3 position, const char* name) {
		Entity* entity = new Entity(scene, name, position);
		entities.emplace(entity->GetName(), entity);
		return entity;
	}

	void EntityManager::Update() {
		for(auto& it : components) {
			for(int i = 0; i < it.second->Size(); i++) {
				IComponent* component = it.second->AtIndex(i);

				if(component->IsActive())
					component->OnProcess();
			}
		}
	}

	Entity* const EntityManager::GetEntity(std::string name) const {
		auto entity = entities.find(name);
		assert(entity != entities.end());

		return (*entity).second;
	}

	void EntityManager::RegisterComponent(IComponent* component) {
		assert(component != nullptr);

		int id = component->GetID(); // component->GetEntity()->GetEntityID();

		auto iter = components.find(id);
		if(iter == components.end()) {
			components.emplace(id, new ComponentArray(component));
		} else {
			(*iter).second->Add(component);
		}

		// TODO: Might change this to start of the first frame rather than on init?
		component->OnStart();
	}
}