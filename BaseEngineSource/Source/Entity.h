#pragma once
#include <unordered_map>
#include "EntityManager.h"
#include "Transform.h"
#include "IComponent.h"
#include "Scene.h"
#include "Logging.h"

namespace BE {
	class Camera;
	class MeshRenderer;
	class Entity {
	public:
		Entity(Scene* scene, const char* name, glm::vec3 position);
		Entity(Scene* scene, const char* name, const float& x, const float& y, const float& z);
		~Entity();

		inline unsigned int GetEntityID() const { return entityID; }
		inline Scene* GetScene() const { return scene; }
		inline std::string GetName() const { return name; }
		inline bool IsActive() const { return isEnabled; }
		inline void SetName(const char* name) { this->name = name; }
		inline void SetName(const std::string& name) { this->name = name; }
		inline void SetEnabled(const bool& isEnabled) { this->isEnabled = isEnabled; }

		template<class T> 
		inline T* GetComponent() const {
			return scene->GetEntityManager().GetEntityComponent<T>(entityID);
		}

		inline void GetComponents(std::vector<IComponent*>& components) {
			return scene->GetEntityManager().GetEntityComponents(components, entityID);
		}

		template<class T, typename... Args>
		T* AddComponent(Args&&... args) {
			assert(!HasComponent<T>());

			// Check if component is a renderer, entities can only have 1 renderer
			// TODO: Check if outdated
			if(std::is_base_of<MeshRenderer, T>()) {
				assert(hasRenderer == false);
				hasRenderer = true;
			}

			// Instantiate Component
			T* comp = new T(std::forward<Args>(args)...);
			comp->entity = this;

			// Register Component
			scene->GetEntityManager().RegisterComponent(comp);

			return comp;
		}

		void AddComponent(IComponent* component) 
		{
			if(component == nullptr)
				return;
			
			BE_ASSERT(!HasComponent(component->GetID()), "Entity - Already has this component");
			
			if (!component->SetOwner(this)) {
				BE_ERROR("Entity - Component already has an owner, cannot add to entity");
				return;
			}

			scene->GetEntityManager().RegisterComponent(component);
		}

		template<class T>
		void RemoveComponent() {
			// TODO: Add Functionality
			// TODO: Check if component is IRender
		}
		
		template<class T>
		inline bool HasComponent() {
			return scene->GetEntityManager().HasComponent<T>(entityID);
		}

		inline bool HasComponent(const size_t& componentId) {
			return scene->GetEntityManager().HasComponent(entityID, componentId);
		}

		Transform transform;

		friend bool operator== (const Entity& c1, const Entity& c2);

	private:

		/// <summary> Scene that the entity exists in </summary>
		Scene* scene = nullptr;
		std::string name = "";
		/// <summary> ID of the current entity </summary>
		unsigned int entityID = 0;
		/// <summary> Is active in the scene </summary>
		bool isEnabled = true;
		/// <summary> True if entity has a IRender component, max 1 per entity </summary>
		bool hasRenderer = false;


		/// <summary> The next free entity ID </summary>
		static unsigned int nextID;
	};
}

