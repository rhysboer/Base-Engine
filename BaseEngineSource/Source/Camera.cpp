#include "Camera.h"
#include "Engine.h"

Camera::Camera(const glm::vec3& position, const float& near, const float& far) : isDirty(true), cameraType(CameraType::PERSPECTIVE), zoom(1) {
	this->near = near;
	this->far = far;

	transform = Transform();
	transform.SetPosition(position);
	transform.SetAutoUpdate(false);

	UpdateCamera();
}

Camera::Camera(const Camera& camera) {
	for(int i = 0; i < 6; i++)
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
	this->zoom = camera.zoom;

	// Perspective
	this->fov = camera.fov;

	this->isDirty = camera.isDirty;
}

void Camera::UpdateCamera() {
	if(isDirty || transform.IsDirty()) {
		transform.UpdateTransform();
		isDirty = false;

		view = glm::inverse(transform.ModelMatrix());

		switch(cameraType) {
			case Camera::CameraType::ORTHOGRAPHIC: {
				projection = glm::ortho(left / zoom, right / zoom, bottom / zoom, top / zoom, near, far);
				break;
			}
			case Camera::CameraType::PERSPECTIVE: {
				glm::vec2 res = Engine::GetWindowSize();
				projection = glm::perspective(glm::radians(fov), (res.y == 0) ? 0 : res.x / res.y, near, far);
				break;
			}
		}

		UpdateFrustumPlanes();
	}
}

void Camera::CreateOrthographic(const float& left, const float& right, const float& bottom, const float& top) {
	cameraType = CameraType::ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;

	projection = glm::ortho(left / zoom, right / zoom, bottom / zoom, top / zoom, near, far);
}

void Camera::CreatePerspective(const float& fov) {
	cameraType = CameraType::PERSPECTIVE;
	
	this->fov = fov;

	glm::vec2 res = Engine::GetWindowSize();
	projection = glm::perspective(glm::radians(fov), (res.y == 0) ? 0 : res.x / res.y, near, far);
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

void Camera::SetZoom(const float& zoom) {
	this->zoom = zoom;
	
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