#include "Plane.h"

namespace BaseEngine {
	Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2) {
		glm::vec3 u = point1 - point0;
		glm::vec3 v = point2 - point0;

		normal = glm::normalize(glm::cross(v, u));
		center = (point0 + point1 + point2) / 3.0f;

		right = glm::normalize(glm::cross(normal, point0));

		distance = glm::dot(normal, point0);
	}

	Plane::Plane(const Plane& plane) {
		center = plane.center;
		normal = plane.normal;
		right = plane.right;
		distance = plane.distance;
	}

	Plane::~Plane() {
	}

	void Plane::SetNormal(const glm::vec3& normal) {
		this->normal = normal;
	}

	void Plane::SetPosition(const glm::vec3& position) {
		center = position;
	}
}