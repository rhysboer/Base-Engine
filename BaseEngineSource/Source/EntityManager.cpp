#include "EntityManager.h"
#include "Entity.h"
#include <assert.h>

namespace BE {
	EntityManager::EntityManager(Scene* scene) : scene(scene) {
	}

	EntityManager::~EntityManager() {
	}

    Entity* const EntityManager::CreateEntity() {
		return CreateEntity(nullptr, 0.0f, 0.0f, 0.0f);
    }

	Entity* const EntityManager::CreateEntity(const char* name) {
		return CreateEntity(name, 0.0f, 0.0f, 0.0f);
	}

	Entity* const EntityManager::CreateEntity(glm::vec3 position) {
		return CreateEntity(position.x, position.y, position.z);
	}

	Entity* const EntityManager::CreateEntity(const float& x, const float& y, const float& z) {
		return CreateEntity(nullptr, x, y, z);
	}

	Entity* const EntityManager::CreateEntity(const char* name, glm::vec3 position) {
		return CreateEntity(name, position.x, position.y, position.z);
	}

	Entity* const EntityManager::CreateEntity(const char* name, const float& x, const float& y, const float& z) {
		Entity* entity = new Entity(scene, name, x, y, z);

		// Rename entity
		if (entities.find(entity->GetName()) != entities.end())
			entity->SetName(entity->GetName() + "asd");

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

	void EntityManager::GetEntityComponents(std::vector<IComponent*>& components, const unsigned int& entityID) const
	{
		for (auto& it : this->components) {
			IComponent* comp = it.second->GetFromEntityID(entityID);
			if (comp != nullptr)
				components.push_back(comp);
		}
	}
}