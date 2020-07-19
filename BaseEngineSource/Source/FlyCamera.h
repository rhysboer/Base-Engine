#pragma once
#include "Camera.h"
#include "glm/gtx/quaternion.hpp"

namespace BE {

	class FlyCamera : public Camera {
	public:

		FlyCamera(const glm::vec3& position, const float& near = 0.1f, const float& far = 1000.0f);
		~FlyCamera();

		void Update();

		void SetSpeed(const float& speed);

	private:

		glm::vec3 euler;

		float rotationSpeed;
		float speed;
	};
}
