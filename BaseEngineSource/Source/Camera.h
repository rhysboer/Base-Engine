#pragma once
#include "IComponent.h"
#include "glm/glm.hpp"

#define FRUSTUM_FACES 6

namespace BE {
	class Ray;
	class Camera : public IComponent {
	public:
		enum class CameraType {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
		
		Camera();
		Camera(const CameraType& cameraType, float fovSize = 45.0f);
		~Camera();

		// Change Camera Styles
		void SetOrthographic(const float& left, const float& right, const float& bottom, const float& top);
		void SetOrthographic(const float& orthoSize, const bool& autoResize = false);
		void SetPerspective(const float& fov);

		inline void SetNear(const float& distance) { near = distance; SetDirty(); }
		inline void SetFar(const float& distance) { far = distance; SetDirty(); }
		inline void SetFOV(const float& fov) { this->fov = fov; SetDirty(); }
		inline void SetOrthoSize(const float& size) { this->size = size / 2.0f; SetDirty(); }
		inline void SetDirty() { this->isDirty = true; }
		inline void SetActive(bool toggle) { isActive = toggle; }
		//inline void LookAt(const glm::vec3& point) { transform.LookAt(point); SetDirty(); }

		glm::mat4 GetView();
		inline glm::mat4 GetProjection() { UpdateCamera(); return projection; }
		inline glm::mat4 GetProjectionView() { UpdateCamera(); return projection * GetView(); }

		inline bool IsActive() const { return isActive; }
		inline float GetNear() const { return near; }
		inline float GetFar() const { return far; }
		inline float GetOrthoSize() const { return size; }
		inline float GetAspect() const { return aspect; }
		inline unsigned int GetCameraIndex() const { return cameraIndex; }
		inline CameraType GetType() const { return cameraType; }

		glm::vec3 ScreenSpaceToWorldSpace(const glm::vec2& screenCoord);

		//Ray CreateRayFromMouse();

		bool IsPointInFrustum(const glm::vec3& point);
		//bool IsAABBInFrustum(const AABB& aabb);
		bool IsSphereInFrustum(const glm::vec3& point, const float radius);

	protected:

		// Inherited via IComponent
		virtual void OnStart() override;
		virtual void OnProcess() override;
		virtual void OnDestroy() override;
		inline virtual size_t GetID() const override { return IComponent::GetComponentID<Camera>(); }

		// Update camera
		void UpdateCamera();
		void UpdateFrustumPlanes();

		//Transform transform;

		glm::vec4 frustumPlanes[6];

		//glm::mat4 view;
		glm::mat4 projection;

		CameraType cameraType;

		bool isActive = true;
		float near = 0.1f;
		float far = 1000.0f;
		float fov, aspect; // Perspective
		float left, right, bottom, top, size; // Orthographic
		unsigned int cameraIndex;

		bool isDirty = true;
		bool autoResizeOrtho;
	};
}

