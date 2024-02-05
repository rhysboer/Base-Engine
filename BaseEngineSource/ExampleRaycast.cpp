#include "ExampleRaycast.h"
#include "Scene.h"
#include "Entity.h"
#include "Logging.h"
#include "Gizmos.h"
#include "Input.h"

void ExampleRaycast::OnStart() {
	//camera = GetEntity()->GetScene()->GetEntityManager().GetEntity("Camera")->GetComponent<BE::Camera>();
	camera = GetEntity()->GetComponent<BE::Camera>();
}

void ExampleRaycast::OnProcess() {
	if(camera != nullptr) {
		if(BE::Input::IsMouseKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			ray = BE::Ray::CreateRayFromMousePosition(*camera, BE::Input::MousePosition());
			ray->Fire(GetEntity()->GetScene(), rayHit);
		}
	}

	if (ray != nullptr) {
		BE::Gizmos::SetColour(COLOUR_PURPLE);
		BE::Gizmos::DrawRay(*ray);
		BE::Gizmos::RevertColour();
	}

	if (rayHit.Hit()) {
		BE::Gizmos::SetColour(COLOUR_RED);
		BE::Gizmos::DrawCube(0.2f, rayHit.CollidPoint());
		BE::Gizmos::RevertColour();
	}
}

void ExampleRaycast::OnDestroy() {
}

size_t ExampleRaycast::GetID() const {
	return IComponent::GetComponentID<ExampleRaycast>();
}
