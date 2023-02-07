#include "ExampleComponent.h"
#include "Entity.h"
#include "TimeManager.h"

#include "Gizmos.h"

#include "EventSystem.h"

void ExampleComponent::OnProcess() {
	//GetEntity()->transform.Rotate(glm::vec3(1.4f, glm::sin(BE::Time::TotalTime()) * 3.0f, glm::cos(BE::Time::TotalTime() * 0.3f) * 3.0f), 1.0f);
	GetEntity()->transform.RotateY(BE::Time::DeltaTime() * 25.0f);

	auto entityCamera = GetEntity()->GetScene()->GetEntityManager().GetEntity("Camera");
	GetEntity()->transform.SetPosition(entityCamera->transform.GetPosition());


	//BE::Gizmos::DrawTransform(GetEntity()->transform.ModelMatrix(), 5);
}

void ExampleComponent::OnStart() {
	//auto ass = std::mem_fn(Event_Callback);
	
	//auto ass = std::bind(&ExampleComponent::Event_Callback, this);
	//unsigned int id = BE::EventSystem::SubEvent("WindowResize", BE_EVENT_FUNC(Event_Callback));

	//auto func = &ExampleComponent::Event_Callback;
	//
	//BE::EventSystem::Add<ExampleComponent>("Test", &ExampleComponent::Event_Callback);
	//BE::EventSystem::SubEvent("Test", std::mem_fn(this->Event_Callback));



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
