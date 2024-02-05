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

		/// <summary> Updates the global Camera buffer </summary>
		void BindBuffer();
		static void BindBuffer(const glm::mat4& proj, const glm::mat4& view);

		inline void SetNear(const float& distance) { near = distance; SetDirty(); }
		inline void SetFar(const float& distance) { far = distance; SetDirty(); }
		inline void SetFOV(const float& fov) { this->fov = fov; SetDirty(); }
		inline void SetOrthoSize(const float& size) { this->size = size / 2.0f; SetDirty(); }
		inline void SetDirty() { this->isDirty = true; }
		inline void SetActive(bool toggle) { isActive = toggle; }
		inline void SetMainCamera() { mainCamera = this; }
		//inline void LookAt(const glm::vec3& point) { transform.LookAt(point); SetDirty(); }

		glm::mat4 GetView();
		inline glm::mat4 GetProjection() { UpdateCamera(); return projection; }
		inline glm::mat4 GetProjectionView() { UpdateCamera(); return projection * GetView(); }

		inline bool IsActive() const { return isActive; }
		inline float GetNear() const { return near; }
		inline float GetFar() const { return far; }
		inline float GetOrthoSize() const { return size * 2.0f; }
		inline float GetAspect() const { return aspect; }
		//inline unsigned int GetCameraIndex() const { return cameraIndex; }
		inline CameraType GetType() const { return cameraType; }
		static inline Camera* const GetMainCamera() { return mainCamera; }

		// Clip Space
		glm::vec3 ScreenSpaceToWorldSpace(const glm::vec2& screenCoord);
		glm::vec2 WorldSpaceToScreenSpace(const glm::vec3& worldCoord);

		//Ray CreateRayFromMouse();

		bool IsPointInFrustum(const glm::vec3& point);
		//bool IsAABBInFrustum(const AABB& aabb);
		bool IsSphereInFrustum(const glm::vec3& point, const float radius);

		virtual inline const char* GetName() const { return "Camera"; }

	protected:

		// Inherited via IComponent
		virtual void OnStart() override;
		virtual void OnProcess() override;
		virtual void OnDestroy() override;
		inline virtual size_t GetID() const override { return IComponent::GetComponentID<Camera>(); }

#ifdef BE_DEBUG
		virtual void OnComponentDraw() override;
#endif

		// Update camera
		void UpdateCamera();
		void UpdateFrustumPlanes();

		glm::vec4 frustumPlanes[6];

		//glm::mat4 view;
		glm::mat4 projection;

		CameraType cameraType;

		bool isActive = true;
		float near = 0.1f;
		float far = 1000.0f;
		float fov, aspect; // Perspective
		float left, right, bottom, top, size; // Orthographic
		//unsigned int cameraIndex;

		unsigned int eventId;
		bool isDirty = true;

	private:

		static Camera* mainCamera;
	};
}

