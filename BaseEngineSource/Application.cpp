#include "Application.h"

void Application::OnStart() {
	camera = new FlyCamera(glm::vec3(0, 0, 20));

	glm::vec2 size = Engine::GetWindowSize();
	camera->CreatePerspective(60.0f);
}

void Application::OnEnd() {
}

void Application::OnUpdate() {
	camera->Update();


	Gizmos::AddCamera(camera->ProjectionView());


	AABB aabb = AABB(glm::vec3(0), glm::vec3(5));
	Gizmos::DrawAABB(aabb, glm::vec3(0, 1, 0));

	RayCast::Ray ray;
	camera->CreateRay(ray);

	RayCast::RayHit hit = RayCast::RayCastAABB(ray, aabb);

	Gizmos::DrawCube(hit.hitPosition, 0.2f, glm::vec3(1, 0, 0));
}

void Application::OnRender() {
}
