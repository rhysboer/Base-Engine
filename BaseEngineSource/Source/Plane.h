#pragma once
#include "glm/glm.hpp"


namespace BaseEngine {
	class Plane {
	public:

		Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
		Plane(const Plane& plane);
		~Plane();

		void SetNormal(const glm::vec3& normal);
		void SetPosition(const glm::vec3& position);

		inline glm::vec3 GetPosition() const {
			return center;
		}
		inline glm::vec3 GetNormal() const {
			return normal;
		}
		inline glm::vec3 GetRight() const {
			return right;
		}

	protected:

		glm::vec3 center;
		glm::vec3 normal;
		glm::vec3 right;

		float distance;
	};
}
