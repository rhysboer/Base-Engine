#include "DebugMovement.h"
#include "Entity.h"
#include "TimeManager.h"
#include "Input.h"

namespace BE {
	void DebugMovement::OnStart() {
	}

	void DebugMovement::OnProcess() {
		Entity* entity = GetEntity();

		glm::vec3 direction = glm::vec3(0, 0, 0);

		if(Input::IsKeyDown(GLFW_KEY_W))
			direction += entity->transform.GetForward(); //transform.Translate(transform.GetForward() * Time::DeltaTime() * speed);
		if(Input::IsKeyDown(GLFW_KEY_S))
			direction -= entity->transform.GetForward(); //transform.Translate(-transform.GetForward() * Time::DeltaTime() * speed);
		if(Input::IsKeyDown(GLFW_KEY_D))
			direction += entity->transform.GetRight(); //transform.Translate(transform.GetRight() * Time::DeltaTime() * speed);
		if(Input::IsKeyDown(GLFW_KEY_A))
			direction -= entity->transform.GetRight(); //transform.Translate(-transform.GetRight() * Time::DeltaTime() * speed);
		if(Input::IsKeyDown(GLFW_KEY_E))
			direction += entity->transform.GetUp();
		if(Input::IsKeyDown(GLFW_KEY_Q))
			direction -= entity->transform.GetUp();

		glm::vec2 delta = glm::vec2(0);
		if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
			delta = Input::MouseDelta();

		euler.y -= delta.x * 0.005f;
		euler.x -= delta.y * 0.005f;
		euler.x = glm::clamp(euler.x, glm::radians(-89.9f), glm::radians(89.9f));

		entity->transform.SetRotation(euler);

		const float speed = 20.0f;
		entity->transform.Translate(direction * Time::DeltaTime() * speed);
	}

	void DebugMovement::OnDestroy() {
	}

	size_t DebugMovement::GetID() const {
		return IComponent::GetComponentID<DebugMovement>();
	}
}