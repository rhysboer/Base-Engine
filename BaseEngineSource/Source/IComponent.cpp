#include "IComponent.h"
#include "Entity.h"

namespace BE {
	bool IComponent::IsActive() {
		return (isEnabled && entity->IsActive());
	}

	Transform& IComponent::GetTransform() const
	{
		return entity->transform;
	}

	bool IComponent::SetOwner(Entity* entity) {
		if (this->entity != nullptr)
			return false;

		this->entity = entity;
		return true;
	}
}