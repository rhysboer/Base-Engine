#include "Camera.h"
#include "BaseEngine.h"
#include "Raycast.h"
#include "Input.h"
#include "Entity.h"
#include "Scene.h"

namespace BE {
	Camera::Camera() : cameraType(CameraType::PERSPECTIVE), fov(45.0f), size(fov/2.0f) 
	{ }

	Camera::Camera(const CameraType& cameraType, float fovSize) : cameraType(cameraType), fov(fovSize), size(fovSize / 2.0f) 
	{ }

	Camera::~Camera() 
	{ }

	void Camera::OnStart() {
		UpdateCamera();
	}

	void Camera::OnProcess() {
	}

	void Camera::OnDestroy() {
	}

	void Camera::UpdateCamera() {
		if(isDirty) {
			isDirty = false;

			//view = glm::inverse(GetEntity()->transform.ModelMatrix());

			switch(cameraType) {
				case Camera::CameraType::ORTHOGRAPHIC:
				{
					int x, y;
					BaseEngine::GetWindowSize(x, y);

					glm::vec2 res = glm::vec2(x, y);

					aspect = res.y == 0 ? 0 : res.x / res.y;

					float height = 2.0f * size;
					float width = height * aspect;

					this->left = -width / 2.0f;
					this->right = width / 2.0f;
					this->bottom = -height / 2.0f;
					this->top = height / 2.0f;

					projection = glm::ortho(left, right, bottom, top, near, far);
					break;
				}
				case Camera::CameraType::PERSPECTIVE:
				{
					int x, y;
					BaseEngine::GetWindowSize(x, y);
					glm::vec2 res = glm::vec2(x, y);

					this->aspect = (res.y == 0) ? 0 : res.x / res.y;
					projection = glm::perspective(glm::radians(fov), aspect, near, far);
					break;
				}
			}

			UpdateFrustumPlanes();
		}
	}

	void Camera::SetOrthographic(const float& left, const float& right, const float& bottom, const float& top) {
		cameraType = CameraType::ORTHOGRAPHIC;
		autoResizeOrtho = false;

		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;

		projection = glm::ortho(left, right, bottom, top, near, far);
	}

	void Camera::SetOrthographic(const float& orthoSize, const bool& autoResize) {
		cameraType = CameraType::ORTHOGRAPHIC;
		autoResizeOrtho = autoResize;

		int x, y;
		BaseEngine::GetWindowSize(x, y);
		glm::vec2 res = glm::vec2(x, y);

		aspect = res.y == 0 ? 0 : res.x / res.y;
		size = orthoSize;

		float height = 2.0f * orthoSize;
		float width = height * aspect;

		this->left = -width / 2.0f;
		this->right = width / 2.0f;
		this->bottom = -height / 2.0f;
		this->top = height / 2.0f;

		projection = glm::ortho(left, right, bottom, top, near, far);
	}

	void Camera::SetPerspective(const float& fov) {
		cameraType = CameraType::PERSPECTIVE;

		int x, y;
		BaseEngine::GetWindowSize(x, y);
		glm::vec2 res = glm::vec2(x, y);

		this->aspect = (res.y == 0) ? 0 : res.x / res.y;
		this->fov = fov;

		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	glm::mat4 Camera::GetView() {
		return glm::inverse(GetEntity()->transform.ModelMatrix());
	}

	glm::vec3 Camera::ScreenSpaceToWorldSpace(const glm::vec2& screenCoord) {
		UpdateCamera();
	
		int x, y;
		BaseEngine::GetWindowSize(x, y);
		glm::vec2 screenSize = glm::vec2(x, y);
		
		if(screenSize.x == 0 || screenSize.y == 0)
			return glm::vec3(0);
	
		//// Convert cursor cordinates to normalised device coordinates. (-1, 1)
		glm::vec3 mouseNDC = glm::vec3(
			(2.0f * screenCoord.x) / screenSize.x - 1.0f,
			1.0f - (2.0f * screenCoord.y) / screenSize.y,
			-1.0f
		);
	
		Entity* entity = GetEntity();
	
		switch(cameraType) {
			case BE::Camera::CameraType::ORTHOGRAPHIC:
				return entity->transform.GetPosition() + (mouseNDC * glm::vec3(right, top, 1));
			case BE::Camera::CameraType::PERSPECTIVE:
				glm::mat4 inverse = glm::inverse(projection * GetView());
				glm::vec4 result = inverse * glm::vec4(mouseNDC.x, mouseNDC.y, 1.0f, 1.0f);
				return result / result.w;
			default:
				return glm::vec3(0);
		}
	}

	//Ray Camera::CreateRayFromMouse() {
	//	UpdateCamera();
	//
	//	glm::vec3 world = ScreenSpaceToWorldSpace(Input::MousePosition());
	//	return Ray(GetEntity()->transform.GetPosition(), glm::normalize(world));
	//
	//
	//	//int x, y;
	//	//BaseEngine::GetWindowSize(x, y);
	//	//glm::vec2 screenSize = glm::vec2(x, y);
	//	//glm::vec2 mousePos = Input::MousePosition();
	//	//
	//	//screenSize.x = glm::max(1.0f, screenSize.x);
	//	//screenSize.y = glm::max(1.0f, screenSize.y);
	//	//
	//	//// Convert cursor cordinates to normalised device coordinates. (-1, 1)
	//	//glm::vec3 mouseNDC = glm::vec3(
	//	//	(2.0f * mousePos.x) / screenSize.x - 1.0f,
	//	//	1.0f - (2.0f * mousePos.y) / screenSize.y,
	//	//	1.0f
	//	//);
	//	//
	//	//// Homogeneous clip coordinates
	//	//glm::vec4 ray_clip = glm::vec4(mouseNDC.x, mouseNDC.y, -1.0f, 1.0f);
	//	//
	//	//// To Eye coordinates
	//	//glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	//	//ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	//	//
	//	//// To World Coordinates
	//	//glm::vec3 ray_world = glm::inverse(view) * ray_eye;
	//	//
	//	//if(cameraType == CameraType::PERSPECTIVE) {
	//	//	ray.origin = transform.GetPosition();
	//	//	ray.direction = glm::normalize(ray_world);
	//	//} else {
	//	//	ray.origin = transform.GetPosition() + ray_world;
	//	//	ray.direction = transform.GetForward();
	//	//}
	//}

	void Camera::UpdateFrustumPlanes() {
		glm::mat4 projView = projection * GetView();

		// right side
		frustumPlanes[0] = glm::vec4(projView[0][3] - projView[0][0],
									 projView[1][3] - projView[1][0],
									 projView[2][3] - projView[2][0],
									 projView[3][3] - projView[3][0]);

		// left side
		frustumPlanes[1] = glm::vec4(projView[0][3] + projView[0][0],
									 projView[1][3] + projView[1][0],
									 projView[2][3] + projView[2][0],
									 projView[3][3] + projView[3][0]);

		// top
		frustumPlanes[2] = glm::vec4(projView[0][3] - projView[0][1],
									 projView[1][3] - projView[1][1],
									 projView[2][3] - projView[2][1],
									 projView[3][3] - projView[3][1]);

		// bottom
		frustumPlanes[3] = glm::vec4(projView[0][3] + projView[0][1],
									 projView[1][3] + projView[1][1],
									 projView[2][3] + projView[2][1],
									 projView[3][3] + projView[3][1]);

		// far
		frustumPlanes[4] = glm::vec4(projView[0][3] - projView[0][2],
									 projView[1][3] - projView[1][2],
									 projView[2][3] - projView[2][2],
									 projView[3][3] - projView[3][2]);

		// near
		frustumPlanes[5] = glm::vec4(projView[0][3] + projView[0][2],
									 projView[1][3] + projView[1][2],
									 projView[2][3] + projView[2][2],
									 projView[3][3] + projView[3][2]);

		// plane normalisation, based on length of normal
		for(int i = 0; i < FRUSTUM_FACES; i++) {
			frustumPlanes[i] /= glm::length(glm::vec3(frustumPlanes[i]));
		}
	}


	//bool Camera::IsPointInFrustum(const glm::vec3& point) {
	//	UpdateCamera();
	//
	//	for(int i = 0; i < FRUSTUM_FACES; i++) {
	//		if(glm::dot(glm::vec3(frustumPlanes[i]), point) + frustumPlanes[i].w < 0.0f)
	//			return false;
	//	}
	//
	//	return true;
	//}

	//bool Camera::IsAABBInFrustum(const AABB& aabb) {
	//	UpdateCamera();
	//
	//	int out = 0;
	//	int in = 0;
	//
	//	for(int i = 0; i < FRUSTUM_FACES; i++) {
	//
	//		out = 0;
	//		in = 0;
	//
	//		for(int v = 0; v < 8; v++) {
	//			if(glm::dot(glm::vec3(frustumPlanes[i]), aabb.GetPoint(v)) + frustumPlanes[i].w < 0.0f) {
	//				out++;
	//			} else {
	//				in++;
	//			}
	//		}
	//
	//		if(in == 0)
	//			return false;
	//	}
	//
	//	return true;
	//}
}