#include "IComponent.h"
#include "Entity.h"

namespace BE {
	bool IComponent::IsActive() {
		return (isEnabled && entity->IsActive());
	}
	//void IComponent::Register(Entity* entity) {
	//	this->entity = entity;
	//	ECSManager::RegisterComponent(this);
	//
	//	OnStart();
	//}
}