#include "ComponentArray.h"
#include "Entity.h"
#include <assert.h>

namespace BE {
	void ComponentArray::Add(IComponent* component) {
		assert(endPos < MAX_COMPONENTS);

		components[endPos] = component;
		indexes.emplace(component->GetEntity()->GetEntityID(), endPos);

		++endPos;
	}

	void ComponentArray::Remove(IComponent* component) {
		// TODO
	}

    IComponent* ComponentArray::AtIndex(const unsigned int& index) const {
		assert(index >= 0 && index < endPos);
		return components[index];
    }

    IComponent* ComponentArray::GetFromEntityID(const unsigned int& entityID) const {
		auto iter = indexes.find(entityID);
		return (iter != indexes.end()) ? components[(*iter).second] : nullptr;
	}
}