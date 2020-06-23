#include "FlyCamera.h"
#include "Engine.h"

FlyCamera::FlyCamera(const glm::vec3& position, const float& near, const float& far) : Camera(position, near, far), euler(0.0f), speed(10.0f), rotationSpeed(0.005f) {
}

FlyCamera::~FlyCamera() {
}

void FlyCamera::Update() {
	if(Input::IsKeyDown(GLFW_KEY_E))
		transform.Translate(transform.GetUp() * Time::DeltaTime() * speed);
	if(Input::IsKeyDown(GLFW_KEY_Q))
		transform.Translate(-transform.GetUp() * Time::DeltaTime() * speed);

	if(Input::IsKeyDown(GLFW_KEY_W))
		transform.Translate(transform.GetForward() * Time::DeltaTime() * speed);
	if(Input::IsKeyDown(GLFW_KEY_S))
		transform.Translate(-transform.GetForward() * Time::DeltaTime() * speed);
	if(Input::IsKeyDown(GLFW_KEY_D))
		transform.Translate(transform.GetRight() * Time::DeltaTime() * speed);
	if(Input::IsKeyDown(GLFW_KEY_A))
		transform.Translate(-transform.GetRight() * Time::DeltaTime() * speed);

	glm::vec2 delta = glm::vec2(0);
	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
		delta = Input::MouseDelta();

	euler.y -= delta.x * rotationSpeed;
	euler.x -= delta.y * rotationSpeed;
	euler.x = glm::clamp(euler.x, glm::radians(-89.9f), glm::radians(89.9f));

	transform.SetRotation(euler);
}

void FlyCamera::SetSpeed(const float& speed) {
	this->speed = speed;
}
