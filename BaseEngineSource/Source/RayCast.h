#pragma once
#include "glm/vec3.hpp"

namespace RayCast {
	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float distance;
	};
}
