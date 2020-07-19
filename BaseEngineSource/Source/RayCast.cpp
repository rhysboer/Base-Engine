#include "RayCast.h"

namespace BE {
	Raycast::RayHit Raycast::RayCastPlane(const Ray& ray, const Plane& plane) {
		float denom = glm::dot(plane.GetNormal(), ray.direction);
		if(glm::abs(denom) > 0.0001f) {
			float t = glm::dot(plane.GetPosition() - ray.origin, plane.GetNormal()) / denom;
			if(t >= 0) {
				return Raycast::RayHit(true, glm::vec3(ray.origin + ray.direction * t));
			}
		}

		return Raycast::RayHit(false, glm::vec3(0));
	}

	// Source: https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	Raycast::RayHit Raycast::RayCastAABB(const Ray& ray, const AABB& aabb) {
		glm::vec3 dirFrac = glm::vec3(
			1.0f / ((ray.direction.x == 0.0f) ? FLT_MIN : ray.direction.x),
			1.0f / ((ray.direction.y == 0.0f) ? FLT_MIN : ray.direction.y),
			1.0f / ((ray.direction.z == 0.0f) ? FLT_MIN : ray.direction.z)
		);

		float t1 = (aabb.GetMin().x - ray.origin.x) * dirFrac.x;
		float t2 = (aabb.GetMax().x - ray.origin.x) * dirFrac.x;
		float t3 = (aabb.GetMin().y - ray.origin.y) * dirFrac.y;
		float t4 = (aabb.GetMax().y - ray.origin.y) * dirFrac.y;
		float t5 = (aabb.GetMin().z - ray.origin.z) * dirFrac.z;
		float t6 = (aabb.GetMax().z - ray.origin.z) * dirFrac.z;

		float tMin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		float tMax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		if(tMax < 0)
			return Raycast::RayHit(false, glm::vec3(0));

		if(tMin > tMax)
			return Raycast::RayHit(false, glm::vec3(0));

		return Raycast::RayHit(true, ray.origin + ray.direction * tMin);
	}
}