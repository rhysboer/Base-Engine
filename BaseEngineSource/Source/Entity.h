#pragma once
#include <unordered_map>
#include "EntityManager.h"
#include "Transform.h"
#include "IComponent.h"
#include "Scene.h"

namespace BE {
	class Camera;
	class IRender;
	class Entity {
	public:
		Entity(Scene* scene, const char* name = nullptr, glm::vec3 position = glm::vec3(0));
		~Entity();

		inline unsigned int GetEntityID() const { return entityID; }
		inline Scene* GetScene() const { return scene; }
		inline std::string GetName() const { return name; }
		inline bool IsActive() const { return isEnabled; }

		template<class T> 
		inline T* GetComponent() const {
			return scene->GetEntityManager().GetEntityComponent<T>(entityID);
		}

		template<class T, typename... Args>
		T* AddComponent(Args&&... args) {
			assert(!HasComponent<T>());

			// Check if component is a renderer, entities can only have 1 renderer
			// TODO: Check if outdated
			if(std::is_base_of<IRender, T>()) {
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

		template<class T>
		void RemoveComponent() {
			// TODO: Add Functionality
			// TODO: Check if component is IRender
		}
		
		template<class T>
		inline bool HasComponent() {
			return scene->GetEntityManager().HasComponent<T>(entityID);
		}

		Transform transform;

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

