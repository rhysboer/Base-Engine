#include "EntityManager.h"
#include "Entity.h"
#include "MeshLoader.h"
#include "MeshRenderer.h"
#include "Logging.h"

namespace BE {
	EntityManager::EntityManager(Scene* scene) : scene(scene) {
	}

	EntityManager::~EntityManager() {
		for (auto iter = entities.begin(); iter != entities.end(); iter++) {
			DeleteEntityComponents((*iter).second->GetEntityID());
			delete (*iter).second;
		}

		entities.clear();
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
			entity->SetName(entity->GetName() + "_"); // TODO: Fix

		entities.emplace(entity->GetName(), entity);
		return entity;
	}

	Entity* const EntityManager::CreateCamera(const char* name, const float& x, const float& y, const float& z)
	{
		Entity* entity = new Entity(scene, name, x, y, z);

		entity->AddComponent<BE::Camera>();

		// TODO: Check for duplicate names
		entities.emplace(name, entity);

		return entity;
	}

	Entity* const EntityManager::CreateCamera(const char* name, glm::vec3 position)
	{
		return CreateCamera(name, position.x, position.y, position.z);
	}

	Entity* const EntityManager::CreatePrimitive(const char* name, const PrimitiveType& type)
	{
		Entity* entity = CreateEntity(name);

		switch (type)	
		{
		case PrimitiveType::BOX_STATIC:
		case PrimitiveType::BOX_RIGIDBODY:
			entity->AddComponent<MeshRenderer>(BE::MeshLoader::CreateCube(1));
			break;
		case PrimitiveType::PLANE:
			entity->AddComponent<MeshRenderer>(BE::MeshLoader::CreatePlane(1));
			break;
		case PrimitiveType::SPHERE_RIGIDBODY:
		case PrimitiveType::SPHERE_STATIC:
			entity->AddComponent<MeshRenderer>(BE::MeshLoader::CreateSphere(1));
			break;
		}

		return entity;
	}

	Entity* const EntityManager::GetEntity(std::string name) const
	{
		auto entity = entities.find(name);
		if (entity != entities.end())
			return (*entity).second;
		return nullptr;
	}

	void EntityManager::Update() {
		int size;
		for(auto& it : components) {
			int size = it.second->Size();
			for(int i = 0; i < size; i++) {
				IComponent* component = it.second->AtIndex(i);

				if (component->IsActive()) {
					component->OnProcess();
				}
			}
		}
	}

	void EntityManager::PhysicsStep(const float& dt)
	{
		int size;
		for (auto& it : components) {
			size = it.second->Size();
			for (int i = 0; i < size; i++) {
				IComponent* component = it.second->AtIndex(i);

				if (component->IsActive()) {
					component->OnPhysicsStep(dt);
				}
			}
		}
	}

	//Entity* const EntityManager::GetEntity(std::string name) const {
	//	auto entity = entities.find(name);
	//	assert(entity != entities.end());
	//
	//	return (*entity).second;
	//}

	void EntityManager::RegisterComponent(IComponent* component) {
		BE_ASSERT(component != nullptr, "Component is null");

		size_t id = component->GetID(); // component->GetEntity()->GetEntityID();

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

	void EntityManager::DeleteEntityComponents(const unsigned int& entityId)
	{
		for (auto& it : this->components) {
			IComponent* comp = it.second->GetFromEntityID(entityId);
			if (comp != nullptr) {
				it.second->Remove(comp);
			}
		}
	}
}