#pragma once
#include "ICollider.h"

namespace BE {
	class PlaneCollider : public ICollider {
	public:
		PlaneCollider(const glm::vec2& size);
		PlaneCollider(const float& x, const float& y);
		PlaneCollider(const float& size);
		~PlaneCollider() {};

		inline void SetSize(const glm::vec2& newSize) { size = newSize; }

		virtual void DebugDraw() override;

	protected:

		virtual void OnProcess() override;

	private:

		glm::vec2 size;

		// Inherited via ICollider
		virtual void OnStart() override;
		virtual void OnDestroy() override;

		virtual bool CheckCollision(const ICollider* collider) override;
		virtual bool CheckRayCollision(const Ray& ray, RayHit& hitOutput) override;
	};
}

