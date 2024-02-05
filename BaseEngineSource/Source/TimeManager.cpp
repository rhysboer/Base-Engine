#include "TimeManager.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace BE {
	float Time::deltaTime = 0.0f;
	float Time::sinceLastUpdate = 0.0f;
	double Time::totalTime = 0.0f;
	float Time::physicsTime = 0.0f;

	void Time::Update() {
		deltaTime = glfwGetTime() - sinceLastUpdate;
		sinceLastUpdate = glfwGetTime();

		totalTime += deltaTime;
	}

	float Time::DeltaTime() {
		return deltaTime;
	}

	double Time::TotalTime() {
		return totalTime;
	}
}