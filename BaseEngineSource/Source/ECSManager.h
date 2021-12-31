/*
#pragma once
#include <unordered_map>
#include <array>
#include <assert.h>
#include "IComponent.h"

#define MAX_COMPONENTS 128

namespace BE {
	class IRender;
	class Entity;

	class ComponentArray {
	public:
		ComponentArray() {};
		ComponentArray(IComponent* component) { Add(component); }
		~ComponentArray() {};

		void Add(IComponent* component);

		inline void Remove(IComponent* component) {
			// TODO
		}

		inline IComponent* Get(const unsigned int& entityID) {
			auto iter = indexes.find(entityID);
			assert(iter != indexes.end());
 
			return components[(*iter).second];
		}

		inline bool Has(const unsigned int& entityID) {
			return indexes.contains(entityID);
		}

	private:

		/// <summary> Holds the index for the component with the given Entity (Uint: EntityID, UInt: Component Index) </summary>
		std::unordered_map<unsigned int, unsigned int> indexes{};

		/// <summary> All components of the same type </summary>
		std::array<IComponent*, MAX_COMPONENTS> components{};

		unsigned int endPos = 0;
	};

	class ECSManager {
		friend Entity;
		friend IComponent;
	public:

		static void ProcessComponents();

	private:

		template<class T>
		static inline IComponent* GetComponent(const unsigned int& entityId) {
			auto iter = components.find(IComponent::GetComponentID<T>());
			assert(iter != components.end());
		
			return (*iter).second.Get(entityId);
		}

		template<class T>
		static inline bool HasCompoment(const unsigned int& entityID);

		static void RegisterComponent(IComponent* component);

		/// <summary> List of all components in the game (size_t: ??, ComponentArray: All components of type) </summary>
		static std::unordered_map<size_t, ComponentArray> components;
	};


	template<class T>
	inline bool ECSManager::HasCompoment(const unsigned int& entityID) {
		auto iter = components.find(IComponent::GetComponentID<T>());
		if(iter == components.end())
			return false;
		return (*iter).second.Has(entityID);
	}
}
*/