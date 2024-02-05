#include "ExampleComponent.h"
#include "Entity.h"
#include "TimeManager.h"

#include "Gizmos.h"

#include "EventSystem.h"

void ExampleComponent::OnProcess() {
	GetEntity()->transform.RotateY(BE::Time::DeltaTime() * 10.0f);
}

void ExampleComponent::OnStart() {

}

void ExampleComponent::OnDestroy() {
}

size_t ExampleComponent::GetID() const {
	return IComponent::GetComponentID<ExampleComponent>();
}

void ExampleComponent::Event_Callback(const void* const data)
{
	glm::vec2 size = *(glm::vec2*)data;

	BE_LOG("Resize: x - %f, y - %f", size.x, size.y);
}
