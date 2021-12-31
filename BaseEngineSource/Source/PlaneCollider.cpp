#include "PlaneCollider.h"
#include "Entity.h"
#include "Raycast.h"

#if _DEBUG
#include "Gizmos.h"
#endif

#include <iostream>

namespace BE {
	PlaneCollider::PlaneCollider(const glm::vec2& size) : ICollider(ColliderType::PLANE), size(size) { }
	PlaneCollider::PlaneCollider(const float& x, const float& y) : ICollider(ColliderType::PLANE), size(x, y) { }
	PlaneCollider::PlaneCollider(const float& size) : ICollider(ColliderType::PLANE), size(size, size) { }

	void PlaneCollider::OnProcess() {

	}

	// Source: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
    bool PlaneCollider::CheckRayCollision(const Ray& ray, RayHit& hitOutput) {
		Entity* entity = GetEntity();
		
		float tMin = 0.0f;
		float tMax = 1000000.0f;

		glm::vec3 aabbMax = glm::vec3(size.x / 2.0f, 0.0f, size.y / 2.0f);
		glm::vec3 aabbMin = aabbMax * -1.0f;

		glm::vec3 position = entity->transform.GetPosition();
		glm::vec3 delta = position - ray.GetOrigin();
		glm::mat4 model = entity->transform.ModelMatrix();

		hitOutput.SetResults(false);

		for(int i = 0; i < 3; i++) {
			glm::vec3 axis = glm::vec3(model[i].x, model[i].y, model[i].z);
			float e = glm::dot(axis, delta);
			float f = glm::dot(ray.GetDirection(), axis);

			float t1 = (e + aabbMin[i]) / f;
			float t2 = (e + aabbMax[i]) / f;

			if(t1 > t2) {
				float w = t1; 
				t1 = t2;
				t2 = w;
			}

			if(t2 < tMax)
				tMax = t2;
			if(t1 > tMin)
				tMin = t1;

			if(tMax < tMin)
				return false;
		}
	
		hitOutput.SetResults(true, ray.GetOrigin() + (ray.GetDirection() * tMin), this);
		return true;
    }

    void PlaneCollider::OnStart() {
    }

    void PlaneCollider::OnDestroy() {
    }

    bool PlaneCollider::CheckCollision(const ICollider* collider) {
        return false;
    }

	void PlaneCollider::DebugDraw() {
		//BE::Gizmos::DrawPlane(size, GetEntity()->transform.ModelMatrix());
	}
}