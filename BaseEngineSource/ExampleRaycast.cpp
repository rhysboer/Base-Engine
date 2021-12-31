#include "ExampleRaycast.h"
#include "Scene.h"
#include "Entity.h"
#include "Logger.h"
#include "Gizmos.h"
#include "Input.h"

void ExampleRaycast::OnStart() {
	//camera = GetEntity()->GetScene()->GetEntityManager().GetEntity("Camera")->GetComponent<BE::Camera>();
	camera = GetEntity()->GetComponent<BE::Camera>();
}

void ExampleRaycast::OnProcess() {
	if(camera != nullptr) {
		if(BE::Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_LEFT)) {
			ray = BE::Ray::CreateRayFromMousePosition(*camera, BE::Input::MousePosition());
			ray->Fire(GetEntity()->GetScene(), rayHit);
		}
	}

	if(ray != nullptr)
		BE::Gizmos::DrawRay(*ray, COLOUR_PURPLE);

	if(rayHit.Hit())
		BE::Gizmos::DrawCube(0.2f, rayHit.CollidPoint(), COLOUR_RED);
}

void ExampleRaycast::OnDestroy() {
}

size_t ExampleRaycast::GetID() const {
	return IComponent::GetComponentID<ExampleRaycast>();
}
