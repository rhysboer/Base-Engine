#pragma once
#include "IComponent.h"

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

		// Inherited via IComponent
		virtual void OnStart() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnProcess() = 0;
		inline size_t GetID() const;
		

		/// <summary> Returns true if collision occurred</summary>
		virtual bool CheckRayCollision(const Ray& ray, RayHit& hitOutput) = 0;

		virtual bool CheckCollision(const ICollider* collider /*TODO: Collision Resolve Return*/) = 0;
		
		/// <summary> Draw collision bounds for debugging </summary>
		virtual void DebugDraw() = 0;

		inline ColliderType GetColliderType() const { return colliderType; }

	private:

		ColliderType colliderType;
	};

	inline size_t ICollider::GetID() const {
		return IComponent::GetComponentID<ICollider>();
	}
}

