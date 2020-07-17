#include "Camera.h"
#include "Engine.h"

namespace BaseEngine {

	Camera::Camera(const glm::vec3& position, const float& near, const float& far) : isDirty(true), cameraType(CameraType::PERSPECTIVE) {
		this->near = near;
		this->far = far;

		transform = Transform();
		transform.SetPosition(position);
		transform.SetAutoUpdate(false);

		UpdateCamera();
	}

	Camera::Camera(const Camera& camera) {
		for(int i = 0; i < FRUSTUM_FACES; i++)
			this->frustumPlanes[i] = camera.frustumPlanes[i];

		this->view = camera.view;
		this->projection = camera.projection;
		this->near = camera.near;
		this->far = camera.far;

		this->cameraType = camera.cameraType;

		// Orthographic
		this->left = camera.left;
		this->right = camera.right;
		this->bottom = camera.bottom;
		this->top = camera.top;
		this->size = camera.size;

		// Perspective
		this->fov = camera.fov;
		this->aspect = camera.aspect;

		this->isDirty = camera.isDirty;
		this->autoResizeOrtho = camera.autoResizeOrtho;
	}

	void Camera::UpdateCamera() {
		if(isDirty || transform.IsDirty()) {
			transform.UpdateTransform();
			isDirty = false;

			view = glm::inverse(transform.ModelMatrix());

			switch(cameraType) {
				case Camera::CameraType::ORTHOGRAPHIC:
				{
					if(autoResizeOrtho == true) {
						glm::vec2 res = Engine::GetWindowSize();
						aspect = res.y == 0 ? 0 : res.x / res.y;

						float height = 2.0f * size;
						float width = height * aspect;

						this->left = -width / 2.0f;
						this->right = width / 2.0f;
						this->bottom = -height / 2.0f;
						this->top = height / 2.0f;
					}

					projection = glm::ortho(left, right, bottom, top, near, far);
					break;
				}
				case Camera::CameraType::PERSPECTIVE:
				{
					glm::vec2 res = Engine::GetWindowSize();

					this->aspect = (res.y == 0) ? 0 : res.x / res.y;
					projection = glm::perspective(glm::radians(fov), aspect, near, far);
					break;
				}
			}

			UpdateFrustumPlanes();
		}
	}

	void Camera::CreateOrthographic(const float& left, const float& right, const float& bottom, const float& top) {
		cameraType = CameraType::ORTHOGRAPHIC;
		autoResizeOrtho = false;

		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;

		projection = glm::ortho(left, right, bottom, top, near, far);
	}

	void Camera::CreateOrthographic(const float& orthoSize, const bool& autoResize) {
		cameraType = CameraType::ORTHOGRAPHIC;
		autoResizeOrtho = autoResize;

		glm::vec2 res = Engine::GetWindowSize();
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

	void Camera::CreatePerspective(const float& fov) {
		cameraType = CameraType::PERSPECTIVE;

		glm::vec2 res = Engine::GetWindowSize();

		this->aspect = (res.y == 0) ? 0 : res.x / res.y;
		this->fov = fov;

		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void Camera::SetNear(const float& distance) {
		near = distance;

		SetDirty();
	}

	void Camera::SetFar(const float& distance) {
		far = distance;

		SetDirty();
	}

	void Camera::SetFOV(const float& fov) {
		this->fov = glm::radians(fov);

		SetDirty();
	}

	void Camera::SetDirty() {
		isDirty = true;
	}

	glm::mat4 Camera::View() {
		UpdateCamera();
		return view;
	}

	glm::mat4 Camera::Projection() {
		UpdateCamera();
		return projection;
	}

	glm::mat4 Camera::ProjectionView() {
		UpdateCamera();
		return projection * view;
	}

	Transform& Camera::GetTransform() {
		return transform;
	}

	float Camera::GetNear() const {
		return near;
	}

	float Camera::GetFar() const {
		return far;
	}

	float Camera::GetOrthoSize() const {
		return size;
	}

	float Camera::GetAspect() const {
		return aspect;
	}

	bool Camera::CreateRay(Raycast::Ray& ray) {
		UpdateCamera();

		glm::vec2 screenSize = Engine::GetWindowSize();
		glm::vec2 mousePos = Input::MousePosition();

		if(screenSize.x == 0 || screenSize.y == 0)
			return false;

		// Convert cursor cordinates to normalised device coordinates. (-1, 1)
		glm::vec3 mouseNDC = glm::vec3(
			(2.0f * mousePos.x) / screenSize.x - 1.0f,
			1.0f - (2.0f * mousePos.y) / screenSize.y,
			1.0f
		);

		// Homogeneous clip coordinates
		glm::vec4 ray_clip = glm::vec4(mouseNDC.x, mouseNDC.y, -1.0f, 1.0f);

		// To Eye coordinates
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		// To World Coordinates
		glm::vec3 ray_world = glm::inverse(view) * ray_eye;

		if(cameraType == CameraType::PERSPECTIVE) {
			ray.origin = transform.GetPosition();
			ray.direction = glm::normalize(ray_world);
		} else {
			ray.origin = transform.GetPosition() + ray_world;
			ray.direction = transform.GetForward();
		}

		return true;
	}

	void Camera::UpdateFrustumPlanes() {
		glm::mat4 projView = projection * view;

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

	bool Camera::IsPointInFrustum(const glm::vec3& point) {
		UpdateCamera();

		for(int i = 0; i < FRUSTUM_FACES; i++) {
			if(glm::dot(glm::vec3(frustumPlanes[i]), point) + frustumPlanes[i].w < 0.0f)
				return false;
		}

		return true;
	}

	bool Camera::IsAABBInFrustum(const AABB& aabb) {
		UpdateCamera();

		int out = 0;
		int in = 0;

		for(int i = 0; i < FRUSTUM_FACES; i++) {

			out = 0;
			in = 0;

			for(int v = 0; v < 8; v++) {
				if(glm::dot(glm::vec3(frustumPlanes[i]), aabb.GetPoint(v)) + frustumPlanes[i].w < 0.0f) {
					out++;
				} else {
					in++;
				}
			}

			if(in == 0)
				return false;
		}

		return true;
	}
}