#pragma once
#include "glm/glm.hpp"
#include <array>

#define BE_BOUNDING_BOX_SIZE 8

namespace BE {
	class BoundingBox;
	typedef BoundingBox AABB;
	typedef BoundingBox OBB;


	class MeshData;
	class Transform;
	class BoundingBox
	{
	public:
	
		BoundingBox();
		BoundingBox(const MeshData& mesh);
		BoundingBox(const glm::vec3& min, const glm::vec3& max);
		~BoundingBox() {} 
	
		void Create(const MeshData& mesh);
		inline void Create(const glm::vec3& min, const glm::vec3& max);

		void Clear();

		inline const glm::vec3& GetMin() const { return isValid ? min : glm::vec3(0); }
		inline const glm::vec3& GetMax() const { return isValid ? max : glm::vec3(0); }

		/// <param name="points">size of BE_BOUNDING_BOX_SIZE (8)</param>
		void GetAABB(const glm::mat4& mat, glm::vec3* const points) const;
		/// <param name="points">size of BE_BOUNDING_BOX_SIZE (8)</param>
		void GetOBB(const glm::mat4& mat, glm::vec3* const points) const;

	private:
	
		bool isValid;
		glm::vec3 min;
		glm::vec3 max;
	};

	inline void BoundingBox::Create(const glm::vec3& min, const glm::vec3& max)
	{
		isValid = true;
		this->min = min;
		this->max = max;
	}
}

