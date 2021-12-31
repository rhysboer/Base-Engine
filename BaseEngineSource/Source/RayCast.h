#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace BE {
	class ICollider;
	class Camera;
	class Scene;
	class RayHit;

	class Ray {
	public:
		static Ray* CreateRayFromMousePosition(Camera& camera, const glm::vec2& mousePosition, const float& distance = 100.0f);
		static Ray* CreateRayFromPosition(const glm::vec3& origin, const glm::vec3& direction, const float& distance = 100.0f);

		~Ray() { };

		void Fire(Scene * const scene, RayHit& output) const;

		inline void SetDistance(const float& newDistance) { distance = newDistance; }

		inline glm::vec3 GetOrigin() const { return origin; }
		inline glm::vec3 GetDirection() const { return direction; }
		inline glm::vec3 GetEnd() const { return origin + (direction * distance); }
		inline float GetDistance() const { return distance; }

	private:
		Ray(const glm::vec3& origin, const glm::vec3& direction, const float& distance = 100.0f);

		glm::vec3 origin;
		glm::vec3 direction;
		float distance;
	};

	class RayHit {
	public:
		RayHit();
		~RayHit() {};

		void SetResults(bool wasHit, const glm::vec3& point = glm::vec3(0), const ICollider* collider = nullptr);

		inline bool Hit() const { return wasHit; }
		inline glm::vec3 CollidPoint() const { return point; }
		inline ICollider const* Collider() const { return collider; }

	private:

		bool wasHit;
		glm::vec3 point;
		ICollider const* collider = nullptr;
	};
}
