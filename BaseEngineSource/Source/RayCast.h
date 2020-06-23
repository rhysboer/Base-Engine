#pragma once
#include "glm/vec3.hpp"
#include "Plane.h"
#include "AABB.h"

namespace RayCast {
	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float distance;
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
