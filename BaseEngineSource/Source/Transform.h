#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace BaseEngine {

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

		void LookAt(const glm::vec3& point);

		void Translate(const glm::vec3& direction);

		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetForward() const;

		glm::mat4 ModelMatrix();

		void SetDirty();
		bool IsDirty() const;
		void SetAutoUpdate(bool option);

		void UpdateTransform();

	private:

		bool isDirty;
		bool autoUpdate;

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::mat4 model;
	};
}