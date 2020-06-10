#include "Transform.h"

Transform::Transform() : translation(1), rotation(0,0,0,1), scale(1), model(1), isDirty(false), parent(nullptr) { }

Transform::Transform(const Transform& other) {
	this->translation = other.translation;
	this->rotation = other.rotation;
	this->scale = other.scale;
	this->parent = other.parent;
	
	SetDirty();
}

Transform::~Transform() { }

void Transform::SetPosition(const glm::vec3& position) {
	SetDirty();
	
	translation = glm::translate(glm::mat4(1), position);
}

void Transform::SetScale(const glm::vec3& scale) {
	SetDirty();

	this->scale = glm::scale(glm::mat4(1), scale);
}

void Transform::SetScale(const float& scale) {
	SetDirty();

	this->scale = glm::scale(glm::mat4(1), glm::vec3(scale));
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

//void Transform::RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis) {
//	SetDirty();
//
//	
//	//glm::mat4 lookAt = glm::lookAt(position, glm::vec3(point.x, position.y, point.z), axis);
//	//rotation = glm::toQuat(glm::inverse(lookAt));
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
	
	this->translation = glm::translate(translation, direction);
}

void Transform::SetDirty() {
	isDirty = true;
}

glm::vec3 Transform::GetPosition() const {
	return translation[3];
}

glm::mat4 Transform::ModelMatrix() {
	UpdateTransform();

	return model;
}

void Transform::UpdateTransform() {
	model = translation * glm::toMat4(rotation) * scale;
	isDirty = false;
}


