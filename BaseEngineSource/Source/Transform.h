#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform {
public:
	Transform();
	Transform(const Transform& other);
	~Transform();

	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetScale(const float& scale);
	void SetRotation(const glm::vec3& euler);
	void SetRotation(const glm::quat& rotation);

	// Rotate
	void RotateX(const float& degree);
	void RotateY(const float& degree);
	void RotateZ(const float& degree);
	void Rotate(const glm::vec3& axis, const float& degree);
	//void RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis);

	void SetParent(Transform* parent);

	void Translate(const glm::vec3& direction);

	glm::vec3 GetPosition() const;
	glm::mat4 ModelMatrix();

	void SetDirty();

private:
	void UpdateTransform();

	bool isDirty;

	glm::mat4 translation;
	glm::quat rotation;
	glm::mat4 scale;
	glm::mat4 model;

	Transform* parent;
};

