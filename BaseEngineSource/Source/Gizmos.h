#pragma once
#include <vector>
#include "glm/glm.hpp"

#define COLOUR_RED glm::vec3(1, 0, 0)
#define COLOUR_GREEN glm::vec3(0, 1, 0)
#define COLOUR_BLUE glm::vec3(0, 0, 1)
#define COLOUR_MAGENTA glm::vec3(1, 0, 1)
#define COLOUR_YELLOW glm::vec3(1, 1, 0)
#define COLOUR_CYAN glm::vec3(0, 1, 1)
#define COLOUR_ORANGE glm::vec3(1, .5f, 0)
#define COLOUR_PURPLE glm::vec3(.5f, 0, .5f)
#define COLOUR_WHITE glm::vec3(1, 1, 1)
#define COLOUR_GREY glm::vec3(.5f, .5f, .5f)
#define COLOUR_BLACK glm::vec3(0, 0, 0)

namespace BE {
	class Mesh;
	class Ray;
	class Gizmos {
	public:

		/*
			TODO:
				- Add a "fill" flag to functions (wireframe)
				- Add a Text function
		*/

		static void Init();
		static void Render(const glm::mat4& cameraMatrix);
		static void Destroy(); // TODO

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawLines(const std::vector<float>& positions, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawLines(const std::vector<glm::vec3>& positions, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawCube(const float& size, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawCube(const float& size, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawSphere(const glm::vec3& center, const float& diameter, const int& stack = 12, const int& sector = 18, const glm::vec3 colour = COLOUR_GREY);
		static void DrawIcosphere(const glm::vec3& center, const float& diameter, const int& subdivision = 1, const glm::vec3& colour = COLOUR_GREY);
		static void DrawCircle(const float& diameter, const int& vertices, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawCircle(const float& diameter, const int& vertices, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawPlane(const float& size, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawPlane(const float& size, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE 
		static void DrawPlane(const glm::vec2& size, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawPlane(const glm::vec2& size, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawGrid(const float& size, const unsigned int& totalLines, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawGrid(const float& size, const unsigned int& totalLines, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawTransform(const glm::mat4& transform, const float& size = 1); // DONE

		static void DrawRay(const Ray& ray, const glm::vec3& colour = COLOUR_GREY);
		//static void DrawPlane(const Plane& plane, const glm::vec3& normalColour = COLOUR_GREY, const glm::vec3& colour = COLOUR_GREY);
		//static void DrawAABB(const AABB& aabb, const glm::vec3& colour = COLOUR_GREY);

	private:
		struct GizmoData {
			Mesh* mesh;

			glm::mat4 transform;
			unsigned int indexOffset;
			std::vector<float> vertexData = std::vector<float>();
			std::vector<unsigned int> indices = std::vector<unsigned int>();

			void Clear() {
				vertexData.clear();
				indices.clear();
				indexOffset = 0;
			}

			void SetTransform(const glm::mat4& mat) {
				transform = mat;
			}

			void AddPoint(glm::vec3 point, glm::vec3 colour) {
				point = transform * glm::vec4(point.x, point.y, point.z, 1);

				vertexData.push_back(point.x);
				vertexData.push_back(point.y);
				vertexData.push_back(point.z);
				vertexData.push_back(colour.x);
				vertexData.push_back(colour.y);
				vertexData.push_back(colour.z);
			}

			void AddPoint(const float& x, const float& y, const float& z, const float& r, const float& g, const float& b) {
				AddPoint(glm::vec3(x, y, z), glm::vec3(r, g, b));
			}

			void AddPoint(const float& x, const float& y, const float& z, const glm::vec3& colour) {
				AddPoint(glm::vec3(x, y, z), colour);
			}

			void AddIndices(const unsigned int& index) {
				indices.push_back(indexOffset + index);
			}

			void AddIndices(const unsigned int& x, const unsigned int& y) {
				indices.push_back(indexOffset + x);
				indices.push_back(indexOffset + y);
			}

			void AddIndices(const unsigned int& x, const unsigned int& y, const unsigned int& z) {
				indices.push_back(indexOffset + x);
				indices.push_back(indexOffset + y);
				indices.push_back(indexOffset + z);
			}

			void AddOffset(const unsigned int& offset) {
				indexOffset += offset;
			}
		};

		static unsigned int shaderId;
		static GizmoData triangles;
		static GizmoData lines;
	};
}
