#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#define CHECK_DIRTY if(isDirty) UpdateTransform(); 

namespace BE {
	class Entity;
	class Transform {
	public:
		Transform();
		Transform(const glm::vec3& position);
		Transform(const float& x, const float& y, const float& z);
		Transform(const Transform& other);
		~Transform();

		inline void SetPosition(const float& x, const float& y, const float& z) { SetDirty(); position.x = x; position.y = y; position.z = z; }
		//inline void SetPosition(const int& x, const int& y, const int& z) { SetDirty(); position.x = x; position.y = y; position.z = z; }
		inline void SetPosition(const glm::vec3& position) { SetPosition(position.x, position.y, position.z); }
		inline void SetScale(const float& x, const float& y, const float& z) { SetDirty(); scale.x = x; scale.y = y; scale.z = z; }
		inline void SetScale(const glm::vec3& scale) { SetScale(scale.x, scale.y, scale.z); }
		inline void SetScale(const float& scale) { SetScale(scale, scale, scale); };
		void SetRotation(const glm::vec3& euler);
		void SetRotation(const glm::quat& rotation);

		void SetParent(const Entity* const entity) { parent = entity; }

		// Rotate
		void RotateX(const float& degree);
		void RotateY(const float& degree);
		void RotateZ(const float& degree);
		void Rotate(const glm::vec3& axis, const float& degree);
		//void RotateAroundPoint(const glm::vec3& point, const float& angle, const glm::vec3& axis);

		void LookAt(const glm::vec3& point);

		void Translate(const float& x, const float& y, const float& z);
		void Translate(const glm::vec3& offset);

		inline glm::vec3 GetPosition() const { return position; }
		inline glm::quat GetRotation() const { return rotation; }
		inline glm::vec3 GetScale() const { return scale; }
		inline glm::vec3 GetUp() const { return rotation * glm::vec3(0, 1, 0); }
		inline glm::vec3 GetRight() const { return rotation * glm::vec3(1, 0, 0); }
		inline glm::vec3 GetForward() const { return rotation * glm::vec3(0, 0, -1); }

		glm::mat4 ModelMatrix() const;

		void SetDirty();
		bool IsDirty() const;

		glm::mat4 operator=(const Transform& transform) const { return transform.ModelMatrix(); }

	private:
		void UpdateTransform() const;

		mutable bool isDirty;

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		const Entity* parent = nullptr;
		mutable glm::mat4 model;
	};
}