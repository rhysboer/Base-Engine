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
		Entity* const CreateEntity();
		Entity* const CreateEntity(const char* name);
		Entity* const CreateEntity(glm::vec3 position);
		Entity* const CreateEntity(const float& x, const float& y, const float& z);
		Entity* const CreateEntity(const char* name, const float& x, const float& y, const float& z);
		Entity* const CreateEntity(const char* name, glm::vec3 position);
		//Entity* const CreateEntity(PrimitiveEntity type, glm::vec3 position = glm::vec3(0), const char* name = nullptr); // TODO

		void Update();

		/// <summary> Returns all components of type in current scene </summary>
		template<class T>
		ComponentArray* const GetComponents();

		Entity* const GetEntity(std::string name) const;
		inline unsigned int GetEntityCount() const { return entities.size(); }

		inline std::unordered_map<std::string, Entity*>::iterator GetEntityIterBegin() { return entities.begin(); };
		inline std::unordered_map<std::string, Entity*>::iterator GetEntityIterEnd() { return entities.end(); }
	private:

		/// <summary> Registers an entitys component </summary>
		void RegisterComponent(IComponent* component);

		/// <summary> Returns an Entity's component (EntityID: ID of the entity) </summary>
		template<class T>
		T* GetEntityComponent(const unsigned int& entityID) const;

		/// <summary> Returns all entities components </summary>
		void GetEntityComponents(std::vector<IComponent*>& components, const unsigned int& entityID) const;

		/// <summary> Checks if entity has the component (EntityID: ID of the entity) </summary>
		template<class T>
		inline bool HasComponent(const unsigned int& entityId);
		inline bool HasComponent(const unsigned int& entityId, const size_t& componentId);

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
	bool EntityManager::HasComponent(const unsigned int& entityId) {
		return HasComponent(entityId, IComponent::GetComponentID<T>());
	}

	bool EntityManager::HasComponent(const unsigned int& entityId, const size_t& componentId) {
		auto iter = components.find(componentId);
		if (iter == components.end())
			return false;

		return (*iter).second->Has(entityId);
	}
}

