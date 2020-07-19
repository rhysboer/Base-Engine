#pragma once
#include "Transform.h"
#include "AABB.h"
#include "RayCast.h"

#define FRUSTUM_FACES 6

namespace BE {

	class Camera {
	public:
		enum class CameraType {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		Camera(const glm::vec3& position, const float& near = 0.1f, const float& far = 1000.0f);
		Camera(const Camera& camera);
		~Camera() {};

		// Change Camera Styles
		void CreateOrthographic(const float& left, const float& right, const float& bottom, const float& top);
		void CreateOrthographic(const float& orthoSize, const bool& autoResize = false);
		void CreatePerspective(const float& fov);

		void SetNear(const float& distance);
		void SetFar(const float& distance);
		void SetFOV(const float& fov);
		void SetDirty();

		glm::mat4 View();
		glm::mat4 Projection();
		glm::mat4 ProjectionView();

		Transform& GetTransform();
		float GetNear() const;
		float GetFar() const;
		float GetOrthoSize() const;
		float GetAspect() const;

		bool CreateRay(Raycast::Ray& ray);

		bool IsPointInFrustum(const glm::vec3& point);
		bool IsAABBInFrustum(const AABB& aabb);
		bool IsSphereInFrustum(const glm::vec3& point, const float radius);

	protected:

		// Update camera
		void UpdateCamera();
		void UpdateFrustumPlanes();

		Transform transform;

		glm::vec4 frustumPlanes[6];

		glm::mat4 view;
		glm::mat4 projection;

		CameraType cameraType;

		float near, far;
		float fov, aspect; // Perspective
		float left, right, bottom, top, size; // Orthographic

		bool isDirty;
		bool autoResizeOrtho;
	};
}

