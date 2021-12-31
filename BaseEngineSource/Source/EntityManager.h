#pragma once
#include <unordered_map>
#include "ComponentArray.h"
#include "glm/glm.hpp"

namespace BE {
	enum class PrimitiveEntity {
		BOX_STATIC,
		BOX_RIGIDBODY,
		SPHERE_STATIC,
		SPHERE_RIGIDBODY,
		PLANE
	};

	class Scene;
	class Entity;
	class EntityManager {
		friend Entity;
	public:

		EntityManager(Scene* scene);
		EntityManager(EntityManager& manager) = delete;
		~EntityManager();

		/// <summary> Creates an empty entity within the scene </summary>
		Entity* const CreateEntity(glm::vec3 position = glm::vec3(0), const char* name = nullptr);
		Entity* const CreateEntity(PrimitiveEntity type, glm::vec3 position = glm::vec3(0), const char* name = nullptr); // TODO

		void Update();

		/// <summary> Returns all components of type in current scene </summary>
		template<class T>
		ComponentArray* const GetComponents();

		Entity* const GetEntity(std::string name) const;

	private:

		/// <summary> Registers an entitys component </summary>
		void RegisterComponent(IComponent* component);

		/// <summary> Returns an Entity's component (EntityID: ID of the entity) </summary>
		template<class T>
		T* GetEntityComponent(const unsigned int& entityID) const;

		/// <summary> Checks if entity has the component (EntityID: ID of the entity) </summary>
		template<class T>
		bool HasComponent(unsigned int& entityID);

	private:

		/// <summary> Scene entites belong to </summary>
		Scene* scene;

		/// <summary> List of entities inside the scene </summary>
		std::unordered_map<std::string, Entity*> entities = std::unordered_map<std::string, Entity*>();

		/// <summary> List of all components in the game (uint: Component ID, ComponentArray: All components of type) </summary>
		std::unordered_map<unsigned int, ComponentArray*> components = std::unordered_map<unsigned int, ComponentArray*>();
	};

	template<class T>
	ComponentArray* const EntityManager::GetComponents() {
		unsigned int id = IComponent::GetComponentID<T>();
		auto iter = components.find(IComponent::GetComponentID<T>());

		if(iter == components.end())
			return nullptr;

		return (*iter).second;
	}

	template<class T>
	T* EntityManager::GetEntityComponent(const unsigned int& entityID) const {
		auto iter = components.find(IComponent::GetComponentID<T>());
		
		if(iter == components.end())
			return nullptr;

		return (T*)((*iter).second->GetFromEntityID(entityID));
	}

	template<class T>
	bool EntityManager::HasComponent(unsigned int& entityID) {
		auto iter = components.find(IComponent::GetComponentID<T>());
		if(iter == components.end())
			return false;

		return (*iter).second->Has(entityID);
	}
}

