#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>

namespace BE {
	enum class ColliderType {
		BOX,
		PLANE,
		SPHERE,
		MESH
		// CAPSULE
	};

	class Ray;
	class RayHit;

	class ICollider : public IComponent {
	public:

		ICollider(ColliderType type);
		~ICollider() { /* TODO */ };

		inline void SetOffset(const float& x, const float& y, const float& z) { offset.x = x; offset.y = y; offset.z = z; }
		inline void SetOffset(const glm::vec3& offset) { SetOffset(offset.x, offset.y, offset.z); }

		// Inherited via IComponent
		virtual void OnStart() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnProcess() = 0;
		virtual inline const char* GetName() const = 0;
		inline size_t GetID() const;
		

		/// <summary> Returns true if collision occurred</summary>
		virtual bool CheckRayCollision(const Ray& ray, RayHit& hitOutput) = 0;
		virtual bool CheckCollision(const ICollider* collider /*TODO: Collision Resolve Return*/) = 0;
		
		/// <summary> Draw collision bounds for debugging </summary>
		virtual void DebugDraw() = 0;

		inline ColliderType GetColliderType() const { return colliderType; }

		glm::vec3 offset {0,0,0};
	protected:


	private:

		ColliderType colliderType;
	};

	inline size_t ICollider::GetID() const {
		return IComponent::GetComponentID<ICollider>();
	}
}

