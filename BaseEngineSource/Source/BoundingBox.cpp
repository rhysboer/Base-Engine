#include "BoundingBox.h"
#include "MeshData.h"

namespace BE {

	BoundingBox::BoundingBox() : min(0), max(0), isValid(false)
	{ }

	BoundingBox::BoundingBox(const MeshData& mesh)
	{ 
		Create(mesh);
	}

	BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
	{ 
		Create(min, max);
	}

	void BoundingBox::Create(const MeshData& mesh)
	{
		isValid = true;
		min = glm::vec3(0x7FFFFFFF);
		max = glm::vec3(-0x7FFFFFFF);

		glm::vec3 pos;
		for (int i = 0; i < mesh.position.size(); i+=3) {
			pos.x = mesh.position[i + 0];
			pos.y = mesh.position[i + 1];
			pos.z = mesh.position[i + 2];

			max = glm::max(pos, max);
			min = glm::min(pos, min);
		}
	}

	void BoundingBox::Clear()
	{
		isValid = false;
		min = glm::vec3(0.0f);
		max = glm::vec3(0.0f);
	}

	// https://stackoverflow.com/questions/6053522/how-to-recalculate-axis-aligned-bounding-box-after-translate-rotate/58630206#58630206
	void BoundingBox::GetAABB(const glm::mat4& mat, glm::vec3* const points) const
	{
		glm::vec3 outMin = mat[3];
		glm::vec3 outMax = mat[3];

		float a, b;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				a = mat[j][i] * min[j];
				b = mat[j][i] * max[j];

				outMin[i] += (a < b) ? a : b;
				outMax[i] += (a < b) ? b : a;
			}
		}

		points[0] = { outMax.x, outMax.y, outMax.z };
		points[1] = { outMin.x, outMax.y, outMax.z };
		points[2] = { outMin.x, outMax.y, outMin.z };
		points[3] = { outMax.x, outMax.y, outMin.z };
		points[4] = { outMax.x, outMin.y, outMax.z };
		points[5] = { outMin.x, outMin.y, outMax.z };
		points[6] = { outMin.x, outMin.y, outMin.z };
		points[7] = { outMax.x, outMin.y, outMin.z };
	}

	void BoundingBox::GetOBB(const glm::mat4& mat, glm::vec3* const points) const
	{
		points[0] = mat *  glm::vec4(max.x, max.y, max.z, 1);
		points[1] = mat *  glm::vec4(min.x, max.y, max.z, 1);
		points[2] = mat *  glm::vec4(min.x, max.y, min.z, 1);
		points[3] = mat *  glm::vec4(max.x, max.y, min.z, 1);
		points[4] = mat *  glm::vec4(max.x, min.y, max.z, 1);
		points[5] = mat *  glm::vec4(min.x, min.y, max.z, 1);
		points[6] = mat *  glm::vec4(min.x, min.y, min.z, 1);
		points[7] = mat *  glm::vec4(max.x, min.y, min.z, 1);
	}
}