#pragma once
#include <vector>
#include "Camera.h"
#include "Object3D.h"
#include "RayCast.h"
#include "Plane.h"
#include "AABB.h"

#define BASECOLOUR glm::vec3(0.5f)

namespace BE {

	class Gizmos {
	public:

		static void Init();
		static void Render();
		static void AddCamera(const glm::mat4& cameraMatrix);

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& colour = BASECOLOUR);
		static void DrawCube(const glm::vec3& position, const float& size, const glm::vec3& colour = BASECOLOUR);

		static void DrawRay(const Raycast::Ray& ray, const glm::vec3& colour = BASECOLOUR);
		static void DrawPlane(const Plane& plane, const glm::vec3& normalColour = BASECOLOUR, const glm::vec3& colour = BASECOLOUR);
		static void DrawAABB(const AABB& aabb, const glm::vec3& colour = BASECOLOUR);

	private:

		static std::vector<Object3D*> gizmos;
		static glm::mat4 projectionView;
		static Shader* shader;
	};
}
