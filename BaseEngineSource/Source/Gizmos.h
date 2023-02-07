#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Mesh.h"

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
#define COLOUR_DARK_RED glm::vec3(0.3f, 0, 0)

namespace BE {
	class BoundingBox;
	class Ray;
	class Transform;
	class Gizmos {
	public:

		/*
			TODO:
				- Add a "fill" flag to functions (wireframe)
				- Add a Text function
		*/

		static void CreateGizmos(const unsigned int& totalBytes = 1000000);
		static void Render(const glm::mat4& cameraMatrix);
		static void Destroy(); // TODO

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& colour = COLOUR_GREY);
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawLine(const std::vector<glm::vec3>& positions, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawLine(const std::vector<glm::vec3>& positions, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawConnectedLines(const std::vector<float>& positions, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawConnectedLines(const std::vector<glm::vec3>& positions, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawConnectedLines(const std::vector<glm::vec3>& positions, const bool& connectToStart, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawThickLine(const glm::vec3& start, const glm::vec3& end, const float& width, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawThickLines(const std::vector<glm::vec3>& positions, const float& width, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); 
		static void DrawCube(const float& size, const glm::vec3& position, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawCube(const float& size, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawSphere(const float& diameter, const glm::vec3& center, const int& stack = 12, const int& sector = 18, const glm::vec3 colour = COLOUR_GREY); // DONE
		static void DrawSphere(const float& diameter, const glm::mat4& mat = glm::mat4(1), const int& stack = 12, const int& sector = 18, const glm::vec3 colour = COLOUR_GREY);
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
		static void DrawTangents(const MeshData& mesh, const Transform& trans, const float& size = 1.0f); // DONE
		static void DrawTangents(const MeshData& mesh, const glm::mat4& mat = glm::mat4(1), const float& size = 1.0f); // DONE
		static void DrawTangents(const MeshData& mesh, const glm::vec3& pos, const float& size = 1.0f); // DONE
		static void DrawRay(const Ray& ray, const glm::vec3& colour = COLOUR_GREY); // DONE
		static void DrawAABB(const BoundingBox& box, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY);
		static void DrawOBB(const BoundingBox& box, const glm::mat4& mat = glm::mat4(1), const glm::vec3& colour = COLOUR_GREY);

		//static void DrawPlane(const Plane& plane, const glm::vec3& normalColour = COLOUR_GREY, const glm::vec3& colour = COLOUR_GREY);
		//static void DrawAABB(const AABB& aabb, const glm::vec3& colour = COLOUR_GREY);

	private:
		struct GizmoData {
			Mesh* mesh;
			MeshData* data;

			glm::mat4 transform;
			unsigned int indexOffset;

			void Clear() {
				data->Clear();
				indexOffset = 0;
			}

			inline void SetTransform(const glm::mat4& mat) {
				transform = mat;
			}

			void AddPoint(glm::vec3 point, glm::vec3 colour) {
				point = transform * glm::vec4(point.x, point.y, point.z, 1);

				data->position.push_back(point.x);
				data->position.push_back(point.y);
				data->position.push_back(point.z);
				
				data->colour0.push_back(colour.x);
				data->colour0.push_back(colour.y);
				data->colour0.push_back(colour.z);
			}

			void AddPoint(const float& x, const float& y, const float& z, const float& r, const float& g, const float& b) {
				AddPoint(glm::vec3(x, y, z), glm::vec3(r, g, b));
			}

			void AddPoint(const float& x, const float& y, const float& z, const glm::vec3& colour) {
				AddPoint(glm::vec3(x, y, z), colour);
			}

			void AddPoint(const std::vector<float>& points, float size = 1.0f) {
				for (int i = 0; i < points.size(); i += 6)
					AddPoint(points[i + 0] * size, points[i + 1] * size, points[i + 2] * size, points[i + 3], points[i + 4], points[i + 5]);
			}

			void AddIndices(const unsigned int& index) {
				data->indices.push_back(indexOffset + index);
			}

			void AddIndices(const unsigned int& x, const unsigned int& y) {
				data->indices.push_back(indexOffset + x);
				data->indices.push_back(indexOffset + y);
			}

			void AddIndices(const unsigned int& x, const unsigned int& y, const unsigned int& z) {
				data->indices.push_back(indexOffset + x);
				data->indices.push_back(indexOffset + y);
				data->indices.push_back(indexOffset + z);
			}

			void AddIndices(const std::vector<unsigned int>& indices) {
				for (int i = 0; i < indices.size(); i++)
					AddIndices(indices[i]);
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
