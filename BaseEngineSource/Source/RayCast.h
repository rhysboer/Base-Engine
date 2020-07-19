#pragma once
#include "glm/vec3.hpp"
#include "Plane.h"
#include "AABB.h"

namespace BE::Raycast {
	struct Ray {
		glm::vec3 origin = glm::vec3(0);
		glm::vec3 direction = glm::vec3(0);
		float distance = 0;

		Ray() {}
		Ray(glm::vec3& origin, glm::vec3& direction, float& distance) : origin(origin), direction(direction), distance(distance) { }
	};

	struct RayHit {
		bool hit;
		glm::vec3 hitPosition;

		RayHit(const bool& hit, const glm::vec3& hitPos) : hit(hit), hitPosition(hitPos) { }
	};

	// Functions
	RayHit RayCastPlane(const Ray& ray, const Plane& plane);
	RayHit RayCastAABB(const Ray& ray, const AABB& aabb);
}
