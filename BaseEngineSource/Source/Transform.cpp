#include "Transform.h"

namespace BE {
	Transform::Transform() : position(0), rotation(glm::identity<glm::quat>()), scale(1), model(1), isDirty(false), autoUpdate(true) {
	}

	Transform::Transform(const Transform& other) {
		this->position = other.position;
		this->rotation = other.rotation;
		this->scale = other.scale;

		SetDirty();
	}

	Transform::~Transform() {}

	void Transform::SetPosition(const glm::vec3& position) {
		SetDirty();

		this->position = position;
	}

	void Transform::SetScale(const glm::vec3& scale) {
		SetDirty();

		this->scale = scale;
	}

	void Transform::SetScale(const float& scale) {
		SetDirty();

		this->scale = glm::vec3(scale);
	}

	void Transform::SetRotation(const glm::vec3& euler) {
		SetDirty();

		this->rotation = glm::quat(euler);
	}

	void Transform::SetRotation(const glm::quat& rotation) {
		SetDirty();

		this->rotation = rotation;
	}

	void Transform::RotateX(const float& degree) {
		SetDirty();

		this->rotation = glm::rotate(this->rotation, glm::radians(degree), glm::vec3(1, 0, 0));
	}

	void Transform::RotateY(const float& angle) {
		SetDirty();

		this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(0, 1, 0));
	}

	void Transform::RotateZ(const float& angle) {
		SetDirty();

		this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(0, 0, 1));
	}

	void Transform::Rotate(const glm::vec3& axis, const float& degree) {
		SetDirty();

		rotation = glm::rotate(rotation, glm::radians(degree), axis);
	}

	void Transform::LookAt(const glm::vec3& point) {
		SetDirty();

		rotation = glm::quat(glm::inverse(glm::lookAt(position, point, GetUp())));
	}

	//void Transform::RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis) {
	//	SetDirty();
	//
	//	
	//	glm::mat4 lookAt = glm::lookAt(position, glm::vec3(point.x, position.y, point.z), axis);
	//	rotation = glm::toQuat(glm::inverse(lookAt));
	//
	//	glm::vec4 direction = glm::vec4(position - point, 1.0);
	//	glm::vec4 rot = glm::rotate(glm::quat(1,0,0,0), glm::radians(angle), axis) * direction;
	//	position = rot + glm::vec4(point, 0.0);
	//}

	//void Transform::SetParent(Transform* parent) {
	//	SetDirty();
	//
	//	this->parent = parent;
	//}

	void Transform::Translate(const glm::vec3& direction) {
		SetDirty();

		this->position += direction;
	}

	void Transform::SetDirty() {
		isDirty = true;
	}

	bool Transform::IsDirty() const {
		return isDirty;
	}

	void Transform::SetAutoUpdate(bool option) {
		autoUpdate = option;
	}

	glm::vec3 Transform::GetPosition() const {
		return position;
	}

	glm::quat Transform::GetRotation() const {
		return rotation;
	}

	glm::vec3 Transform::GetUp() const {
		return rotation * glm::vec3(0, 1, 0);
	}

	glm::vec3 Transform::GetRight() const {
		return rotation * glm::vec3(1, 0, 0);
	}

	glm::vec3 Transform::GetForward() const {
		return rotation * glm::vec3(0, 0, -1);
	}

	glm::mat4 Transform::ModelMatrix() {
		if(autoUpdate)
			UpdateTransform();

		return model;
	}

	void Transform::UpdateTransform() {
		model = glm::translate(glm::mat4(1), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1), scale);
		isDirty = false;
	}
}