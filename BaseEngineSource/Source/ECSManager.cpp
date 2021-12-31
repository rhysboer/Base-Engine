/*
#include "ECSManager.h"
#include "IComponent.h"
#include "Entity.h"
#include <algorithm>

namespace BE {
	std::unordered_map<size_t, ComponentArray> ECSManager::components = std::unordered_map<size_t, ComponentArray>();

	void ECSManager::ProcessComponents() {
	}

	void ECSManager::RegisterComponent(IComponent* component) {
		assert(component != nullptr);

		int id = component->GetEntityID();

		auto iter = components.find(id);
		unsigned int index = 0;
		if(iter == components.end()) {
			components.emplace(id, ComponentArray(component));
		} else {
			(*iter).second.Add(component);
		}
	}

	void ComponentArray::Add(IComponent* component) {
		assert(endPos < MAX_COMPONENTS);

		components[endPos] = component;
		indexes.emplace(component->GetEntity()->GetEntityID(), endPos);

		++endPos;
	}
}
*/