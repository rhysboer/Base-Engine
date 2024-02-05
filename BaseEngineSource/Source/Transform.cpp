#include "Transform.h"
#include "Entity.h"

namespace BE {
	Transform::Transform(Entity* entity) : 
		position(0), rotation(glm::identity<glm::quat>()), scale(1), model(1), isDirty(false), self(entity)
	{}

	Transform::Transform(const glm::vec3& position, Entity* entity) :
		position(position), rotation(glm::identity<glm::quat>()), scale(1), model(1), isDirty(false), self(entity)
	{}

	Transform::Transform(const float& x, const float& y, const float& z, Entity* entity) :
		position(x, y, z), rotation(glm::identity<glm::quat>()), scale(1), model(1), isDirty(false), self(entity)
	{ }

	Transform::Transform(const Transform& other) {
		this->position = other.position;
		this->rotation = other.rotation;
		this->scale = other.scale;

		SetDirty();
	}

	Transform::~Transform() {}

	void Transform::SetRotation(const glm::vec3& euler) {
		SetDirty();
		this->euler = euler;
		this->rotation = glm::quat(euler);
	}

	void Transform::SetRotation(const glm::quat& rotation) {
		SetDirty();
		
		this->rotation = rotation;
	}

	void Transform::SetParent(Entity* const entity)
	{
		// TODO: Check if entity is child
		if (entity == self)
			return;

		SetParent(entity, true);
	}

	void Transform::AddChild(Entity* const entity)
	{
		if (entity == self || parent == entity)
			return;

		AddChild(entity, true);
	}

	void Transform::RotateX(const float& angle) {
		SetRotation(glm::vec3(this->euler.x + glm::radians(angle), this->euler.y, this->euler.z));
	}

	void Transform::RotateY(const float& angle) {
		SetRotation(glm::vec3(this->euler.x, this->euler.y + glm::radians(angle), this->euler.z));
	}

	void Transform::RotateZ(const float& angle) {
		SetRotation(glm::vec3(this->euler.x, this->euler.y, this->euler.z + glm::radians(angle)));
	}

	void Transform::Rotate(const glm::vec3& axis, const float& degree) {
		SetDirty();

		rotation = glm::rotate(rotation, glm::radians(degree), axis);
	}

	void Transform::LookAt(const glm::vec3& point) {
		SetDirty();

		rotation = glm::quat((glm::lookAt(position, point, GetUp())));
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

    void Transform::Translate(const float& x, const float& y, const float& z) {
		SetDirty();

		this->position.x += x;
		this->position.y += y;
		this->position.z += z;
    }

    void Transform::Translate(const glm::vec3& offset) {
		Translate(offset.x, offset.y, offset.z);
	}

	void Transform::Translate(const glm::vec2& offset)
	{
		Translate(offset.x, offset.y, 0.0f);
	}

	void Transform::ForceUpdate() const
	{
		isDirty = true;
		UpdateTransform();
	}

	glm::mat4 Transform::ModelMatrix() const {
		UpdateTransform();
		return (parent != nullptr) ? parent->transform.ModelMatrix() * model : model;
	}

	void Transform::UpdateTransform() const {
		if (isDirty) {
			model = glm::translate(glm::mat4(1), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1), scale);
			isDirty = false;
		}
	}

	void Transform::SetParent(Entity* entity, const bool& callAddChild)
	{
		parent = entity;

		if(callAddChild)
			entity->transform.AddChild(self, false);
	}

	void Transform::AddChild(Entity* entity, const bool& callSetParent)
	{
		const int id = entity->GetEntityID();

		// TODO: Fix this mess, was too tired to implement this correctly
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->GetEntityID() == id)
			{
				return;
			}
		}
		// TODO end

		children.push_back(entity);

		if(callSetParent)
			entity->transform.SetParent(self, false);
	}
}