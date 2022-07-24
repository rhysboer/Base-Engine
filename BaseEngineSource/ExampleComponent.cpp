#include "ExampleComponent.h"
#include "Entity.h"
#include "TimeManager.h"

#include "Gizmos.h"

void ExampleComponent::OnProcess() {
	//GetEntity()->transform.Rotate(glm::vec3(1.4f, glm::sin(BE::Time::TotalTime()) * 3.0f, glm::cos(BE::Time::TotalTime() * 0.3f) * 3.0f), 1.0f);
	GetEntity()->transform.RotateY(BE::Time::DeltaTime() * 100.0f);

	//BE::Gizmos::DrawTransform(GetEntity()->transform.ModelMatrix(), 5);
}

void ExampleComponent::OnStart() {
}

void ExampleComponent::OnDestroy() {
}

size_t ExampleComponent::GetID() const {
	return IComponent::GetComponentID<ExampleComponent>();
}
