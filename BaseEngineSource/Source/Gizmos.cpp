#include "Gizmos.h"
#include <string>
#include "glm/gtx/compatibility.hpp"
#include "ShaderManager.h"
#include "Raycast.h"
#include "Camera.h"
#include "Mesh.h"

namespace BE {
	Gizmos::GizmoData Gizmos::triangles = Gizmos::GizmoData();
	Gizmos::GizmoData Gizmos::lines = Gizmos::GizmoData();
	unsigned int Gizmos::shaderId = 0;

	void Gizmos::Init() {
		std::string vertexSource = "\
			#version 330 core \n\
			layout(location = 0) in vec3 vertex; \n\
			layout(location = 1) in vec3 colour; \n\
			\
			uniform mat4 model;	\n\
			uniform mat4 projectionView; \n\
			out vec3 _colour; \n\
			\
			void main() \n\
			{ \n\
				_colour = colour; \n\
				gl_Position = projectionView * model * vec4(vertex, 1); \n\
			} \n\
		";

		std::string fragmentSource = "\
			#version 330 core \n\
			in vec3 _colour; \n\
			out vec4 FragColor; \n\
			\
			void main() { \n\
				FragColor = vec4(_colour, 1.0); \n\
			} \n\
		";

		shaderId = ShaderManager::AddShaderSource("Gizmos", vertexSource.c_str(), fragmentSource.c_str());

		triangles.mesh = new Mesh({}, { 3, 3 }, {});
		lines.mesh = new Mesh({}, { 3, 3 }, {});
	}

	void Gizmos::Render(const glm::mat4& cameraMatrix) {
		std::vector<unsigned int> attributes = std::vector<unsigned int>({ 3, 3 });

		Shader* shader = ShaderManager::GetShader((int)shaderId);
		shader->UseProgram();
		shader->SetMatrix4("projectionView", cameraMatrix);
		shader->SetMatrix4("model", glm::mat4(1));

		if(triangles.vertexData.size() > 0) {
			triangles.mesh->SetMeshData(0, triangles.vertexData, attributes, triangles.indices);
			triangles.mesh->Apply();

			triangles.mesh->BindVAO();
			glDrawElements(GL_TRIANGLES, triangles.indices.size(), GL_UNSIGNED_INT, 0);
		}

		if(lines.vertexData.size() > 0) {
			lines.mesh->SetMeshData(0, lines.vertexData, attributes, lines.indices);
			lines.mesh->Apply();

			lines.mesh->BindVAO();
			glDrawElements(GL_LINES, lines.indices.size(), GL_UNSIGNED_INT, 0);
		}

		triangles.Clear();
		lines.Clear();
		
		triangles.mesh->Clear();
		lines.mesh->Clear();
	}

	void Gizmos::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::mat4& mat, const glm::vec3& colour) {
		lines.SetTransform(mat);

		lines.AddPoint(start, colour);
		lines.AddPoint(end, colour);
		
		lines.AddIndices(0);
		lines.AddIndices(1);

		lines.AddOffset(2);
	}

	void Gizmos::DrawLines(const std::vector<float>& positions, const glm::mat4& mat, const glm::vec3& colour) {
		const int size = positions.size();

		if(size <= 5) // Needs at least 2 points to draw
			return;

		int offset = positions.size() % 3;
		for(int i = 0; i < size - 3 - offset; i += 3)
			DrawLine(glm::vec3(positions[i + 0], positions[i + 1], positions[i + 2]), glm::vec3(positions[i + 3], positions[i + 4], positions[i + 5]), mat, colour);
	}

	void Gizmos::DrawLines(const std::vector<glm::vec3>& positions, const glm::mat4& mat, const glm::vec3& colour) {
		const int size = positions.size();
		
		if(size <= 1)
			return;
		
		for(int i = 0; i < size - 1; i++)
			DrawLine(positions[i], positions[i + 1], mat, colour);
	}

    void Gizmos::DrawCube(const float& size, const glm::vec3& position, const glm::vec3& colour) {
		DrawCube(size, glm::translate(glm::mat4(1), position), colour);
    }

	void Gizmos::DrawCube(const float& size, const glm::mat4& mat, const glm::vec3& colour) {
		triangles.SetTransform(mat);
		lines.SetTransform(mat);

		// Triangles
		triangles.AddPoint(-0.5f * size,-0.5f * size, 0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint(-0.5f * size,-0.5f * size,-0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint( 0.5f * size,-0.5f * size,-0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint( 0.5f * size,-0.5f * size, 0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint(-0.5f * size, 0.5f * size, 0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint(-0.5f * size, 0.5f * size,-0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint( 0.5f * size, 0.5f * size,-0.5f * size, colour.r, colour.g, colour.b);
		triangles.AddPoint( 0.5f * size, 0.5f * size, 0.5f * size, colour.r, colour.g, colour.b);

		triangles.AddIndices(0, 1, 2);
		triangles.AddIndices(0, 2, 3);
		triangles.AddIndices(0, 1, 5);
		triangles.AddIndices(0, 5, 4);
		triangles.AddIndices(3, 7, 6);
		triangles.AddIndices(3, 6, 2);
		triangles.AddIndices(0, 4, 7);
		triangles.AddIndices(0, 7, 3);
		triangles.AddIndices(1, 5, 6);
		triangles.AddIndices(1, 6, 2);
		triangles.AddIndices(4, 7, 6);
		triangles.AddIndices(4, 6, 5);
		triangles.AddOffset(8);

		// Lines
		lines.AddPoint(-0.5f * size,-0.5f * size, 0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint(-0.5f * size,-0.5f * size,-0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint( 0.5f * size,-0.5f * size,-0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint( 0.5f * size,-0.5f * size, 0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint(-0.5f * size, 0.5f * size, 0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint(-0.5f * size, 0.5f * size,-0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint( 0.5f * size, 0.5f * size,-0.5f * size, 1.0f, 1.0f, 1.0f);
		lines.AddPoint( 0.5f * size, 0.5f * size, 0.5f * size, 1.0f, 1.0f, 1.0f);

		lines.AddIndices(0, 1);
		lines.AddIndices(1, 2);
		lines.AddIndices(2, 3);
		lines.AddIndices(3, 0);
		lines.AddIndices(4, 5);
		lines.AddIndices(5, 6);
		lines.AddIndices(6, 7);
		lines.AddIndices(7, 4);
		lines.AddIndices(0, 4);
		lines.AddIndices(1, 5);
		lines.AddIndices(2, 6);
		lines.AddIndices(3, 7);
		lines.AddOffset(8);
	}

	void Gizmos::DrawSphere(const glm::vec3& center, const float& diameter, const int& stackCount, const int& sectorCount, const glm::vec3 colour) {
		//std::vector<unsigned int> indices = std::vector<unsigned int>();
		//std::vector<glm::vec3> points = std::vector<glm::vec3>();
		//
		//int lineIndexStart = lin_indexCount;
		//
		//const float radius = diameter / 2.0f;
		//const float pi = glm::pi<float>();
		//
		//// Top
		//AddTriPoint(glm::vec3(0, radius, 0) + center, colour);
		//AddLinePoint(glm::vec3(0, radius, 0) + center, COLOUR_WHITE);
		//
		//glm::vec3 previousPos = glm::vec3(0);
		//for(int sector = 0; sector < sectorCount; sector++) { // <=
		//	// Horizontal Line
		//	float hoz = (2.0f * pi) * ((float)sector / sectorCount);
		//	
		//	for(int stack = 1; stack < stackCount; stack++) {
		//
		//		// Vertical Line
		//		float ver = (pi / 2.0f) - pi * ((float)stack / stackCount);
		//
		//		glm::vec3 pos = glm::vec3(
		//			((radius * glm::cos(ver)) * glm::cos(hoz)) + center.x,
		//			(radius * glm::sin(ver)) + center.y,
		//			((radius * glm::cos(ver)) * glm::sin(hoz)) + center.z
		//		);
		//
		//
		//		AddLinePoint(pos, COLOUR_WHITE);
		//
		//		if(stack != stackCount - 1) {
		//			// vertical 
		//			lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1)) + stack);
		//			lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1)) + stack + 1);
		//		
		//			// Horizontal
		//			bool loop = sector == sectorCount - 1;
		//
		//			lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1)) + stack);
		//			if(!loop)
		//				lin_indices.push_back(lineIndexStart + ((sector + 1) * (stackCount - 1)) + stack);
		//			else
		//				lin_indices.push_back(lineIndexStart + stack);
		//
		//			lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1)) + stack + 1);
		//			if(!loop)
		//				lin_indices.push_back(lineIndexStart + ((sector + 1) * (stackCount - 1)) + stack + 1);
		//			else
		//				lin_indices.push_back(lineIndexStart + stack + 1);
		//		}
		//
		//		previousPos = pos;
		//
		//		AddTriPoint(pos, colour);
		//	}
		//
		//	if(sector <= sectorCount) {
		//		lin_indices.push_back(lineIndexStart);
		//		lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1) + 1));
		//		
		//		lin_indices.push_back(lineIndexStart + (sectorCount * (stackCount - 1)) + 1);
		//		lin_indices.push_back(lineIndexStart + (sector * (stackCount - 1) + stackCount - 1));
		//	}
		//}
		//
		//AddLinePoint(glm::vec3(0, -radius, 0) + center, COLOUR_WHITE);
		//AddTriPoint(glm::vec3(0, -radius, 0) + center, colour);
		//
		//lin_indexCount += (sectorCount * (stackCount - 1)) + 2;
		//
		//const int max = (stackCount - 1) * sectorCount + 1;
		//for(int j = 0; j < sectorCount; j++) {
		//	int offset = (stackCount - 1) * j;
		//	int offset2 = (j == sectorCount - 1) ? -(stackCount - 1) : offset;
		//
		//	for(int i = 0; i < stackCount; i++) {
		//		if(i == 0) {
		//			indices.push_back(tri_indexCount + 0);
		//			indices.push_back(tri_indexCount + 1 + offset);
		//			indices.push_back(tri_indexCount + stackCount + offset2);
		//			continue;
		//		} else if(i == stackCount - 1) {
		//			indices.push_back(tri_indexCount + max);
		//			indices.push_back(tri_indexCount + stackCount + i - 1 + offset2);
		//			indices.push_back(tri_indexCount + i + offset);
		//			continue;
		//		}
		//
		//		indices.push_back(tri_indexCount + i + offset);
		//		indices.push_back(tri_indexCount + i + 1 + offset);
		//		indices.push_back(tri_indexCount + stackCount + i - 1 + offset2);
		//
		//		indices.push_back(tri_indexCount + stackCount + i - 1 + offset2);
		//		indices.push_back(tri_indexCount + i + 1 + offset);
		//		indices.push_back(tri_indexCount + stackCount + i + offset2);
		//	}
		//}
		//
		//tri_indices.insert(tri_indices.end(), indices.begin(), indices.end());
		//tri_indexCount += max + 1;
	}

	// Source: http://www.songho.ca/opengl/gl_sphere.html#icosphere
	void Gizmos::DrawIcosphere(const glm::vec3& center, const float& diameter, const int& subdivision, const glm::vec3& colour) {
		//std::vector<float> vertexData = std::vector<float>();
		//
		//constexpr float pi = glm::pi<float>();
		//const float h_angle = glm::radians(360.0f / 5.0f);
		//const float v_angle = glm::atan(1.0f / 2.0f);
		//float radius = diameter / 2.0f;
		//
		//float angle_1 = -pi / 2.0f - h_angle / 2.0f;
		//float angle_2 = -pi / 2.0f;
		//
		//std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
		//std::vector<unsigned int> indexes = std::vector<unsigned int>({ 
		//	0, 1, 3,
		//	0, 3, 5,
		//	0, 5, 7,
		//	0, 7, 9,
		//	0, 9, 1,
		//	
		//	1, 3, 2,
		//	3, 5, 4,
		//	5, 7, 6,
		//	7, 9, 8,
		//	9, 1, 10,
		//
		//	2, 3, 4,
		//	4, 5, 6,
		//	6, 7, 8,
		//	8, 9, 10,
		//	10, 1, 2,
		//
		//	11, 2, 4,
		//	11, 4, 6,
		//	11, 6, 8,
		//	11, 8, 10,
		//	11, 10, 2
		//});
		//
		//vertices.push_back(glm::vec3(0, radius, 0));
		//
		//for(int x = 0; x < 5; x++) {
		//	vertices.push_back(glm::vec3(
		//		radius * glm::cos(v_angle) * glm::cos(angle_1),
		//		radius * glm::sin(v_angle),
		//		radius * glm::cos(v_angle) * glm::sin(angle_1)
		//	));
		//
		//	vertices.push_back(glm::vec3(
		//		radius * glm::cos(v_angle) * glm::cos(angle_2),
		//		radius *-glm::sin(v_angle),
		//		radius * glm::cos(v_angle) * glm::sin(angle_2)
		//	));
		//
		//	angle_1 += h_angle;
		//	angle_2 += h_angle;
		//}
		//
		//vertices.push_back(glm::vec3(0, -radius, 0));
		//
		//std::vector<glm::vec3> tempVertices = std::vector<glm::vec3>();
		//std::vector<unsigned int> tempIndices = std::vector<unsigned int>();
		//
		//int index = 0;
		//for(int i = 0; i < glm::clamp(subdivision, 0, 5); i++) {
		//
		//	tempVertices = vertices;
		//	tempIndices = indexes;
		//	indexes.clear();
		//	vertices.clear();
		//	index = 0;
		//
		//	int total = tempVertices.size();
		//	for(int j = 0; j < tempIndices.size(); j += 3) {
		//
		//		auto v1 = tempVertices[tempIndices[j + 0]];
		//		auto v2 = tempVertices[tempIndices[j + 1]];
		//		auto v3 = tempVertices[tempIndices[j + 2]];
		//
		//		auto newV1 = (v1 + v2) / 2.0f;
		//		auto newV2 = (v2 + v3) / 2.0f;
		//		auto newV3 = (v1 + v3) / 2.0f;
		//
		//		float scale1 = radius / glm::sqrt(newV1[0] * newV1[0] + newV1[1] * newV1[1] + newV1[2] * newV1[2]);
		//		float scale2 = radius / glm::sqrt(newV2[0] * newV2[0] + newV2[1] * newV2[1] + newV2[2] * newV2[2]);
		//		float scale3 = radius / glm::sqrt(newV3[0] * newV3[0] + newV3[1] * newV3[1] + newV3[2] * newV3[2]);
		//
		//		newV1 *= scale1;
		//		newV2 *= scale2;
		//		newV3 *= scale3;
		//
		//		vertices.push_back(v1);
		//		vertices.push_back(newV1);
		//		vertices.push_back(newV3);
		//
		//		vertices.push_back(newV1);
		//		vertices.push_back(v2);
		//		vertices.push_back(newV2);
		//
		//		vertices.push_back(newV1);
		//		vertices.push_back(newV2);
		//		vertices.push_back(newV3);
		//
		//		vertices.push_back(newV3);
		//		vertices.push_back(newV2);
		//		vertices.push_back(v3);
		//
		//		indexes.push_back(index); 	indexes.push_back(index + 1); 	indexes.push_back(index + 2);
		//		indexes.push_back(index + 3); 	indexes.push_back(index + 4); 	indexes.push_back(index + 5);
		//		indexes.push_back(index + 6); 	indexes.push_back(index + 7); 	indexes.push_back(index + 8);
		//		indexes.push_back(index + 9); 	indexes.push_back(index + 10); 	indexes.push_back(index + 11);
		//		index += 12;
		//	}
		//}
		//
		//for(int i = 0; i < vertices.size(); i++) {
		//	vertexData.push_back(vertices[i].x);
		//	vertexData.push_back(vertices[i].y);
		//	vertexData.push_back(vertices[i].z);
		//
		//	vertexData.push_back(0.0f);
		//	vertexData.push_back(0.0f);
		//	vertexData.push_back(0.0f);
		//
		//	vertexData.push_back(colour.x);
		//	vertexData.push_back(colour.y);
		//	vertexData.push_back(colour.z);
		//
		//	vertexData.push_back(0.0f);
		//	vertexData.push_back(0.0f);
		//}
		//
		//Object3D* obj = new Object3D(center, vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }), indexes);
		////obj->shader = shader;
		////obj->SetRenderType(Object3D::RenderType::TRIANGLES);
		////
		////gizmos.push_back(obj);
	}

	void Gizmos::DrawCircle(const float& diameter, const int& vertices, const glm::vec3& position, const glm::vec3& colour) {
		DrawCircle(diameter, vertices, glm::translate(glm::mat4(1), position), colour);
	}

	void Gizmos::DrawCircle(const float& diameter, const int& vertices, const glm::mat4& mat, const glm::vec3& colour) {
		triangles.SetTransform(mat);
		lines.SetTransform(mat);

		triangles.AddPoint(0, 0, 0, colour);

		const float max = glm::clamp(vertices, 3, 64);
		for(int i = 0; i < max; i++) {
			float sin = glm::sin((i / max) * glm::pi<float>() * 2);
			float cos = glm::cos((i / max) * glm::pi<float>() * 2);

			glm::vec3 point = glm::vec3((sin * (diameter * 0.5f)), 0, (cos * (diameter * 0.5f)));

			triangles.AddPoint(point, colour);
			lines.AddPoint(point, COLOUR_WHITE);

			triangles.AddIndices(0, i + 1, i == max - 1 ? 1 : i + 2);
			lines.AddIndices(i, i == max - 1 ? 0 : i + 1);
		}

		triangles.AddOffset(max + 1);
		lines.AddOffset(max);
	}

	void Gizmos::DrawPlane(const float& size, const glm::vec3& position, const glm::vec3& colour) {
		DrawPlane(glm::vec2(size, size), glm::translate(glm::mat4(1), position), colour);
	}

	void Gizmos::DrawPlane(const float& size, const glm::mat4& mat, const glm::vec3& colour) {
		DrawPlane(glm::vec2(size, size), mat, colour);
	}

	void Gizmos::DrawPlane(const glm::vec2& size, const glm::vec3& position, const glm::vec3& colour) {
		DrawPlane(size, glm::translate(glm::mat4(1), position), colour);
	}

	void Gizmos::DrawPlane(const glm::vec2& size, const glm::mat4& mat, const glm::vec3& colour) {
		triangles.SetTransform(mat);
		lines.SetTransform(mat);

		// Triangles
		//triangles.AddPoint(-0.5f * size.x, 0.0f, 0.5f * size.y, colour);
		//triangles.AddPoint(-0.5f * size.x, 0.0f, -0.5f * size.y, colour);
		//triangles.AddPoint(0.5f * size.x, 0.0f, -0.5f * size.y, colour);
		//triangles.AddPoint(0.5f * size.x, 0.0f, 0.5f * size.y, colour);
		//
		//triangles.AddIndices(0, 1, 2);
		//triangles.AddIndices(0, 2, 3);
		//triangles.AddOffset(4);

		// Lines
		lines.AddPoint(-0.5f * size.x, 0.0f, 0.5f * size.y, COLOUR_RED);
		lines.AddPoint(-0.5f * size.x, 0.0f, -0.5f * size.y, COLOUR_RED);
		lines.AddPoint(0.5f * size.x, 0.0f, -0.5f * size.y, COLOUR_RED);
		lines.AddPoint(0.5f * size.x, 0.0f, 0.5f * size.y, COLOUR_RED);

		lines.AddIndices(0, 1);
		lines.AddIndices(1, 2);
		lines.AddIndices(2, 3);
		lines.AddIndices(3, 0);
		lines.AddOffset(4);
	}

	void Gizmos::DrawGrid(const float& size, const unsigned int& totalLines, const glm::vec3& position, const glm::vec3& colour) {
		DrawGrid(size, totalLines, glm::translate(glm::mat4(1), position), colour);
	}

	void Gizmos::DrawGrid(const float& size, const unsigned int& totalLines, const glm::mat4& mat, const glm::vec3& colour) {
		lines.SetTransform(mat);

		unsigned int max = (int)glm::max((float)totalLines, 2.0f);
		unsigned int index = 0;
		float point = 0;
		for(int i = 0; i < max; i++) {
			if((max % 2) == 1 && i == glm::floor(max * 0.5f))
				continue;
			point = glm::lerp(-size, size, i / (float)(max - 1));

			lines.AddPoint(-point, 0, size, colour);
			lines.AddPoint(-point, 0,-size, colour);
			lines.AddIndices(index, index + 1);

			lines.AddPoint( size, 0, -point, colour);
			lines.AddPoint(-size, 0, -point, colour);
			lines.AddIndices(index + 2, index + 3);

			index += 4;
		}

		lines.AddPoint(0, 0, size, COLOUR_RED);
		lines.AddPoint(0, 0,-size, COLOUR_RED);
		lines.AddIndices(index, index + 1);

		lines.AddPoint( size, 0, 0, COLOUR_BLUE);
		lines.AddPoint(-size, 0, 0, COLOUR_BLUE);
		lines.AddIndices(index + 2, index + 3);

		lines.AddOffset(index + 4);
	}

	void Gizmos::DrawTransform(const glm::mat4& transform, const float& size) {
		lines.SetTransform(transform);

		lines.AddPoint(0, 0, 0, COLOUR_RED);
		lines.AddPoint(1 * size, 0, 0, COLOUR_RED);
		lines.AddPoint(0, 0, 0, COLOUR_GREEN);
		lines.AddPoint(0, 1 * size, 0, COLOUR_GREEN);
		lines.AddPoint(0, 0, 0, COLOUR_BLUE);
		lines.AddPoint(0, 0, 1 * size, COLOUR_BLUE);

		lines.AddIndices(0, 1);
		lines.AddIndices(2, 3);
		lines.AddIndices(4, 5);

		lines.AddOffset(6);
    }

	void Gizmos::DrawRay(const Ray& ray, const glm::vec3& colour) {
		DrawLine(ray.GetOrigin(), ray.GetEnd(), glm::mat4(1), colour);
	}

	//void Gizmos::DrawPlane(const Plane& plane, const glm::vec3& normalColour, const glm::vec3& colour) {
	//	//glm::vec3 normal = plane.GetNormal();
	//	//glm::vec3 pos = plane.GetPosition();
	//	//
	//	//float size = 5.0f;
	//	//
	//	//glm::vec3 dirA = plane.GetRight() * size;
	//	//glm::vec3 dirB = glm::cross(plane.GetNormal(), plane.GetRight()) * size;
	//	//
	//	//std::vector<float> vertexData = std::vector<float>({
	//	//	// Direction 1
	//	//	dirA.x, dirA.y * size, dirA.z,			0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
	//	//	-dirA.x, -dirA.y * size, -dirA.z,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
	//	//
	//	//	// Direction 2
	//	//	dirB.x, dirB.y, dirB.z,			0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
	//	//	-dirB.x, -dirB.y, -dirB.z,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
	//	//
	//	//	// Normal
	//	//	0.0f, 0.0f, 0.0f,					0.0f, 0.0f, 0.0f,		normalColour.x, normalColour.y, normalColour.z,		0.0f, 0.0f,
	//	//	normal.x, normal.y, normal.z,		0.0f, 0.0f, 0.0f,		normalColour.x, normalColour.y, normalColour.z,		0.0f, 0.0f,
	//	//												   });
	//	//
	//	//Object3D* obj = new Object3D(pos, vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
	//	////obj->shader = shader;
	//	////obj->SetRenderType(Object3D::RenderType::LINES);
	//	////
	//	////gizmos.push_back(obj);
	//}

	//void Gizmos::DrawAABB(const AABB& aabb, const glm::vec3& colour) {
	//	//std::vector<float> vertexData = std::vector<float>();
	//	//
	//	//auto addData = [&](const glm::vec3& vert, const glm::vec3& norm, const glm::vec3& col, const glm::vec2& uv) {
	//	//	vertexData.push_back(vert.x);
	//	//	vertexData.push_back(vert.y);
	//	//	vertexData.push_back(vert.z);
	//	//
	//	//	vertexData.push_back(norm.x);
	//	//	vertexData.push_back(norm.y);
	//	//	vertexData.push_back(norm.z);
	//	//
	//	//	vertexData.push_back(col.x);
	//	//	vertexData.push_back(col.y);
	//	//	vertexData.push_back(col.z);
	//	//
	//	//	vertexData.push_back(uv.x);
	//	//	vertexData.push_back(uv.y);
	//	//};
	//	//
	//	//const unsigned int corners = 4;
	//	//for(int i = 0; i < corners; i++) {
	//	//	// TOP
	//	//	glm::vec3 pointA = aabb.GetPoint(i);
	//	//	glm::vec3 pointB = aabb.GetPoint((i + 1) % corners);
	//	//
	//	//	addData(pointA, glm::vec3(0), colour, glm::vec2(0));
	//	//	addData(pointB, glm::vec3(0), colour, glm::vec2(0));
	//	//
	//	//	// BOT
	//	//	glm::vec3 pointC = aabb.GetPoint(i + corners);
	//	//	glm::vec3 pointD = aabb.GetPoint(((i + 1) % corners) + corners);
	//	//
	//	//	addData(pointC, glm::vec3(0), colour, glm::vec2(0));
	//	//	addData(pointD, glm::vec3(0), colour, glm::vec2(0));
	//	//
	//	//	// EDGES
	//	//	addData(pointA, glm::vec3(0), colour, glm::vec2(0));
	//	//	addData(pointC, glm::vec3(0), colour, glm::vec2(0));
	//	//}
	//	//
	//	//
	//	//Object3D* obj = new Object3D(aabb.GetOrigin(), vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
	//	////obj->shader = shader;
	//	////obj->SetRenderType(Object3D::RenderType::LINES);
	//	////
	//	////gizmos.push_back(obj);
	//}
}