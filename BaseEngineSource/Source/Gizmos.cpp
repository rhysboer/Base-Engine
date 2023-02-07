#include "Gizmos.h"
#include <glm/gtx/quaternion.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/integer.hpp"
#include "ShaderManager.h"
#include "Raycast.h"
#include "Camera.h"
#include "Transform.h"
#include "BoundingBox.h"

#include "Logging.h"

namespace BE {
	Gizmos::GizmoData Gizmos::triangles = Gizmos::GizmoData();
	Gizmos::GizmoData Gizmos::lines = Gizmos::GizmoData();
	unsigned int Gizmos::shaderId = 0;

	void Gizmos::CreateGizmos(const unsigned int& totalBytes) {
		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 vertex;
			layout(location = 1) in vec3 colour; 
			
			uniform mat4 model;	
			uniform mat4 projectionView; 

			out vec3 _colour; 
			
			void main() 
			{ 
				_colour = colour;
				gl_Position = projectionView * model * vec4(vertex, 1);
			}
		)";

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

		MeshDesc desc = MeshDesc();
		desc.mapData = true;
		desc.maxMapSize = totalBytes;

		triangles.mesh = new Mesh(desc);
		triangles.data = new MeshData();
		triangles.mesh->CreateMesh();

		lines.mesh = new Mesh(desc);
		lines.data = new MeshData();
		lines.mesh->CreateMesh();
	}

	void Gizmos::Render(const glm::mat4& cameraMatrix) {
		std::vector<unsigned int> attributes = std::vector<unsigned int>({ 3, 3 });

		Shader* shader = ShaderManager::GetShader((int)shaderId);
		shader->UseProgram();
		shader->SetMatrix4("projectionView", cameraMatrix);
		shader->SetMatrix4("model", glm::mat4(1));

		if(triangles.data->position.size() > 0) {
			triangles.mesh->UpdateMesh(*triangles.data);
			triangles.mesh->Apply();
			triangles.mesh->BindVAO();

			glDrawElements(GL_TRIANGLES, triangles.mesh->GetIndicesCount(0), GL_UNSIGNED_INT, 0);
		}

		if(lines.data->position.size() > 0) {
			lines.mesh->UpdateMesh(*lines.data);
			lines.mesh->Apply();

			lines.mesh->BindVAO();
			glDrawElements(GL_LINES, lines.data->indices.size(), GL_UNSIGNED_INT, 0);
		}

		triangles.Clear();
		lines.Clear();
		
		triangles.mesh->Clear();
		lines.mesh->Clear();
	}

	void Gizmos::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& colour)
	{
		DrawLine(start, end, glm::mat4(1), colour);
	}

	void Gizmos::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::mat4& mat, const glm::vec3& colour) {
		lines.SetTransform(mat);

		lines.AddPoint(start, colour);
		lines.AddPoint(end, colour);
		
		lines.AddIndices(0);
		lines.AddIndices(1);

		lines.AddOffset(2);
	}

	void Gizmos::DrawLine(const std::vector<glm::vec3>& positions, const glm::vec3& colour)
	{
		DrawLine(positions, glm::mat4(1), colour);
	}

	void Gizmos::DrawLine(const std::vector<glm::vec3>& positions, const glm::mat4& mat, const glm::vec3& colour)
	{
		int size = (positions.size() % 2) == 0 ? positions.size() : glm::max<int>(positions.size() - 1, 0);

		for (int i = 0; i < size; i+=2)
			DrawLine(positions[i], positions[i + 1], mat, colour);
	}

	void Gizmos::DrawConnectedLines(const std::vector<float>& positions, const glm::mat4& mat, const glm::vec3& colour) {
		const int size = positions.size();

		if(size <= 5) // Needs at least 2 points to draw
			return;

		int offset = positions.size() % 3;
		for(int i = 0; i < size - 3 - offset; i += 3)
			DrawLine(glm::vec3(positions[i + 0], positions[i + 1], positions[i + 2]), glm::vec3(positions[i + 3], positions[i + 4], positions[i + 5]), mat, colour);
	}

	void Gizmos::DrawConnectedLines(const std::vector<glm::vec3>& positions, const glm::mat4& mat, const glm::vec3& colour) {
		DrawConnectedLines(positions, false, mat, colour);
	}

	void Gizmos::DrawConnectedLines(const std::vector<glm::vec3>& positions, const bool& connectToStart, const glm::mat4& mat, const glm::vec3& colour)
	{
		const int size = positions.size();

		if (size <= 1)
			return;

		for (int i = 0; i < size - 1; i++)
			DrawLine(positions[i], positions[i + 1], mat, colour);

		if (connectToStart)
			DrawLine(positions[positions.size() - 1], positions[0], mat, colour);
	}

	void Gizmos::DrawThickLine(const glm::vec3& start, const glm::vec3& end, const float& width, const glm::mat4& mat, const glm::vec3& colour)
	{
		triangles.SetTransform(mat);

		glm::vec3 dir1 = glm::normalize(end - start);
		glm::vec3 dir2 = dir1;

		for (int i = 0; i < 3; i++) {
			std::swap(dir1[i], dir1[glm::mod(i + 1, 3)]);
			std::swap(dir2[i], dir2[glm::mod(i - 1, 3)]);
		}

		triangles.AddPoint(start + (dir1 * width), colour);
		triangles.AddPoint(start + (dir1 * -width), colour);
		
		triangles.AddPoint(end + (dir1 * width), colour);
		triangles.AddPoint(end + (dir1 * -width), colour);

		triangles.AddPoint(start + (dir2 * width), colour);
		triangles.AddPoint(start + (dir2 * -width), colour);
		
		triangles.AddPoint(end + (dir2 * width), colour);
		triangles.AddPoint(end + (dir2 * -width), colour);

		// Lines
		triangles.AddIndices(0, 1, 2);
		triangles.AddIndices(1, 2, 3);
		triangles.AddIndices(4, 5, 6);
		triangles.AddIndices(5, 6, 7);

		// Caps
		triangles.AddIndices(0, 1, 4);
		triangles.AddIndices(0, 1, 5);
		triangles.AddIndices(2, 3, 6);
		triangles.AddIndices(2, 3, 7);

		triangles.AddOffset(8);
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

	void Gizmos::DrawSphere(const float& diameter, const glm::mat4& mat, const int& stack, const int& sector, const glm::vec3 colour)
	{
		triangles.SetTransform(mat);
		lines.SetTransform(mat);

		const float pi = 3.1415f;
		const float sectorStep = (2.0f * pi) / (float)sector;
		const float stackStep = pi / (float)stack;

		triangles.AddPoint(0, diameter, 0, colour);
		lines.AddPoint(0, diameter, 0, glm::vec3(1));


		// Stack
		for (int i = 1; i < stack; i++) {
			float stackAngle = pi / 2.0f - i * stackStep;

			// Sector
			for (int j = 0; j < sector; j++) {
				float sectorAngle = j * sectorStep;

				glm::vec3 point = glm::vec3((diameter * glm::cos(stackAngle)) * glm::cos(sectorAngle),
					diameter * glm::sin(stackAngle),
					(diameter * glm::cos(stackAngle)) * glm::sin(sectorAngle));

				triangles.AddPoint(point, colour);
				lines.AddPoint(point, glm::vec3(1));
			}
		}

		triangles.AddPoint(0, -diameter, 0, colour);
		lines.AddPoint(0, -diameter, 0, glm::vec3(1));

		unsigned int p, o;
		int p1, p2, p3, p4;
		for (int i = 0; i < stack - 2; i++) {
			p = (i * sector) + 1;

			for (int j = 0; j < sector; j++) {
				p1 = j;
				p2 = j + 1 >= sector ? 0 : p1 + 1;
				p3 = j + sector;
				p4 = j + 1 >= sector ? sector : p3 + 1;

				if (i == 0) {
					o = j + 2 > sector ? 1 : j + 2;

					triangles.AddIndices(0);
					triangles.AddIndices(j + 1);
					triangles.AddIndices(o);

					lines.AddIndices(0);
					lines.AddIndices(j + 1);
				}

				if (i == sector - 3) {
					o = ((sector - 1) * sector) + 1;
					triangles.AddIndices(((sector - 1) * sector) + 1);
					triangles.AddIndices(p3 + p);
					triangles.AddIndices(p4 + p);

					lines.AddIndices(o);
					lines.AddIndices(p3 + p);

					lines.AddIndices(p3 + p);
					lines.AddIndices(p4 + p);
				}

				triangles.AddIndices(p1 + p);
				triangles.AddIndices(p2 + p);
				triangles.AddIndices(p3 + p);

				triangles.AddIndices(p2 + p);
				triangles.AddIndices(p3 + p);
				triangles.AddIndices(p4 + p);

				lines.AddIndices(p1 + p);
				lines.AddIndices(p2 + p);

				lines.AddIndices(p1 + p);
				lines.AddIndices(p3 + p);
			}
		}

		triangles.AddOffset(((stack - 1) * sector) + 2);
		lines.AddOffset(((stack - 1) * sector) + 2);
	}

	void Gizmos::DrawSphere(const float& diameter, const glm::vec3& center, const int& stack, const int& sector, const glm::vec3 colour) 
	{
		DrawSphere(diameter, glm::translate(glm::mat4(1), center), stack, sector, colour);
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
		glm::mat4 trans = transform;
		trans[0] = glm::normalize(trans[0] / 1.0f);
		trans[1] = glm::normalize(trans[1] / 1.0f);
		trans[2] = glm::normalize(trans[2] / 1.0f);
		
		triangles.SetTransform(trans);

		// X
		triangles.AddPoint({
			-0.000000, 0.050000,  0.000000,	 1, 0, 0, 0.700000,	 0.050000,	0.000000,  1, 0, 0, 0.000000,  0.035355,  -0.035355, 1, 0, 0,
			0.700000,  0.035355,  -0.035355, 1, 0, 0, 0.000000,	 0.000000,	-0.050000, 1, 0, 0, 0.700000,  0.000000,  -0.050000, 1, 0, 0,
			-0.035355, -0.035355, -0.035355, 1, 0, 0, 0.700000,	 -0.035355, -0.035355, 1, 0, 0, 0.000000,  -0.050000, 0.000000,	 1, 0, 0,
			0.700000,  -0.050000, 0.000000,	 1, 0, 0, -0.000000, -0.035355, 0.035355,  1, 0, 0, 0.700000,  -0.035355, 0.035355,	 1, 0, 0,
			-0.000000, 0.000000,  0.050000,	 1, 0, 0, 0.700000,	 0.000000,	0.050000,  1, 0, 0, -0.000000, 0.035355,  0.035355,	 1, 0, 0,
			0.700000,  0.035355,  0.035355,	 1, 0, 0, 0.700000,	 0.053033,	-0.053033, 1, 0, 0, 0.700000,  0.075000,  0.000000,	 1, 0, 0,
			0.700000,  0.000000,  -0.075000, 1, 0, 0, 0.700000,	 -0.053033, -0.053033, 1, 0, 0, 0.700000,  -0.075000, 0.000000,	 1, 0, 0,
			0.700000,  -0.053033, 0.053033,	 1, 0, 0, 0.700000,	 0.000000,	0.075000,  1, 0, 0, 0.700000,  0.053033,  0.053033,	 1, 0, 0,
			1.000000,  0.000000,  0.000000,	 1, 0, 0
			}, size);
		triangles.AddIndices({
			1,	2,	0,	3,	4,	2,	4,	7,	6,	7,	8,	6,	9,	10, 8,	11, 12, 10, 5,	16, 18, 13, 14, 12, 15, 0,	14, 22, 21, 24,
			9,	21, 11, 15, 17, 1,	1,	16, 3,	5,	19, 7,	13, 21, 22, 9,	19, 20, 13, 23, 15, 20, 19, 24, 18, 16, 24, 23, 22, 24,
			21, 20, 24, 19, 18, 24, 16, 17, 24, 17, 23, 24, 1,	3,	2,	3,	5,	4,	4,	5,	7,	7,	9,	8,	9,	11, 10, 11, 13, 12,
			5,	3,	16, 13, 15, 14, 15, 1,	0,	9,	20, 21, 15, 23, 17, 1,	17, 16, 5,	18, 19, 13, 11, 21, 9,	7,	19, 13, 22, 23,
			});
		triangles.AddOffset(25);

		// Y
		triangles.AddPoint({
			-0.050000, 0.000020,  0.000110,	 0, 1, 0, -0.049725, 0.699998,	-0.005532, 0, 1, 0, -0.035447, -0.035624, -0.034991, 0, 1, 0,
			-0.035159, 0.699707,  -0.040919, 0, 1, 0, -0.000110, -0.000403, -0.049998, 0, 1, 0, 0.000164,  0.699574,  -0.055641, 0, 1, 0,
			0.035277,  -0.000299, -0.035432, 0, 1, 0, 0.035552,	 0.699678,	-0.041074, 0, 1, 0, 0.050000,  -0.000020, -0.000110, 0, 1, 0,
			0.050274,  0.699957,  -0.005752, 0, 1, 0, 0.035433,	 0.000270,	0.035276,  0, 1, 0, 0.035708,  0.700248,  0.029634,	 0, 1, 0,
			0.000110,  0.000403,  0.049998,	 0, 1, 0, 0.000384,	 0.700380,	0.044356,  0, 1, 0, -0.035277, 0.000299,  0.035432,	 0, 1, 0,
			-0.035003, 0.700277,  0.029789,	 0, 1, 0, -0.052875, 0.699571,	-0.058557, 0, 1, 0, -0.074725, 0.700008,  -0.005478, 0, 1, 0,
			0.000109,  0.699373,  -0.080640, 0, 1, 0, 0.053191,	 0.699528,	-0.058790, 0, 1, 0, 0.075274,  0.699946,  -0.005807, 0, 1, 0,
			0.053424,  0.700383,  0.047272,	 0, 1, 0, 0.000440,	 0.700582,	0.069355,  0, 1, 0, -0.052642, 0.700426,  0.047505,	 0, 1, 0,
			0.000392,  0.999967,  -0.008060, 0, 1, 0,
		}, size);
		triangles.AddIndices({
			0,  3,  2,  3,  4,  2,  5,  6,  4,  7,  8,  6,  9,  10, 8,  11, 12, 10, 5,  16, 18, 13, 14, 12, 15, 0,  14, 22, 21, 24,
			9,  21, 11, 15, 17, 1,  1,  16, 3,  5,  19, 7,  13, 21, 22, 9,  19, 20, 13, 23, 15, 20, 19, 24, 18, 16, 24, 23, 22, 24,
			21, 20, 24, 19, 18, 24, 16, 17, 24, 17, 23, 24, 0,  1,  3,  3,  5,  4,  5,  7,  6,  7,  9,  8,  9,  11, 10, 11, 13, 12,
			5,  3,  16, 13, 15, 14, 15, 1,  0,  9,  20, 21, 15, 23, 17, 1,  17, 16, 5,  18, 19, 13, 11, 21, 9,  7,  19, 13, 22, 23
		});
		triangles.AddOffset(25);


		// Z
		triangles.AddPoint({
			-0.000000, 0.050000,  -0.000000, 0, 0, 1, 0.000000,	 0.050000,	0.700000,  0, 0, 1, 0.035355,  0.035355,  -0.000000, 0, 0, 1,
			0.035355,  0.035355,  0.700000,	 0, 0, 1, 0.050000,	 0.000000,	-0.000000, 0, 0, 1, 0.050000,  0.000000,  0.700000,	 0, 0, 1,
			0.035355,  -0.035355, -0.000000, 0, 0, 1, 0.035355,	 -0.035355, 0.700000,  0, 0, 1, -0.000000, -0.050000, 0.000000,	 0, 0, 1,
			0.000000,  -0.050000, 0.700000,	 0, 0, 1, -0.035355, -0.035355, -0.035355, 0, 0, 1, -0.035355, -0.035355, 0.700000,	 0, 0, 1,
			-0.050000, 0.000000,  0.000000,	 0, 0, 1, -0.050000, 0.000000,	0.700000,  0, 0, 1, -0.035355, 0.035355,  0.000000,	 0, 0, 1,
			-0.035355, 0.035355,  0.700000,	 0, 0, 1, 0.053033,	 0.053033,	0.700000,  0, 0, 1, 0.000000,  0.075000,  0.700000,	 0, 0, 1,
			0.075000,  0.000000,  0.700000,	 0, 0, 1, 0.053033,	 -0.053033, 0.700000,  0, 0, 1, 0.000000,  -0.075000, 0.700000,	 0, 0, 1,
			-0.053033, -0.053033, 0.700000,	 0, 0, 1, -0.075000, 0.000000,	0.700000,  0, 0, 1, -0.053033, 0.053033,  0.700000,	 0, 0, 1,
			0.000000,  0.000000,  1.000000,	 0, 0, 1
		}, size);
		triangles.AddIndices({
			1,	2,	0,	3,	4,	2,	5,	6,	4,	7,	8,	6,	8,	11, 10, 11, 12, 10, 5,	16, 18, 13, 14, 12, 15, 0,	14, 22, 21, 24,
			9,	21, 11, 15, 17, 1,	1,	16, 3,	5,	19, 7,	13, 21, 22, 9,	19, 20, 13, 23, 15, 20, 19, 24, 18, 16, 24, 23, 22, 24,
			21, 20, 24, 19, 18, 24, 16, 17, 24, 17, 23, 24, 1,	3,	2,	3,	5,	4,	5,	7,	6,	7,	9,	8,	8,	9,	11, 11, 13, 12,
			5,	3,	16, 13, 15, 14, 15, 1,	0,	9,	20, 21, 15, 23, 17, 1,	17, 16, 5,	18, 19, 13, 11, 21, 9,	7,	19, 13, 22, 23,
		});
		triangles.AddOffset(25);
    }

	void Gizmos::DrawTangents(const MeshData& mesh, const Transform& trans, const float& size)
	{
		DrawTangents(mesh, glm::translate(glm::mat4(1), trans.GetPosition()) * glm::toMat4(trans.GetRotation()), size);
	}

	void Gizmos::DrawTangents(const MeshData& mesh, const glm::vec3& pos, const float& size)
	{
		DrawTangents(mesh, glm::translate(glm::mat4(1), pos));
	}

	// TODO: Optimize
	void Gizmos::DrawTangents(const MeshData& mesh, const glm::mat4& mat, const float& size)
	{
		unsigned int offset = 0;
		glm::mat4 trans = glm::translate(glm::mat4(1), (glm::vec3)mat[3]);

		glm::vec3 pos[3];
		glm::vec3 nrm[3];
		glm::vec3 tan[3];

		// TODO: Handle without indices
		while (offset < mesh.indices.size()) {
			tan[0] = glm::normalize(mat * glm::vec4(mesh.tangents[mesh.indices[offset + 0] * 3 + 0], mesh.tangents[mesh.indices[offset + 0] * 3 + 1], mesh.tangents[mesh.indices[offset + 0] * 3 + 2], 0));
			tan[1] = glm::normalize(mat * glm::vec4(mesh.tangents[mesh.indices[offset + 1] * 3 + 0], mesh.tangents[mesh.indices[offset + 1] * 3 + 1], mesh.tangents[mesh.indices[offset + 1] * 3 + 2], 0));
			tan[2] = glm::normalize(mat * glm::vec4(mesh.tangents[mesh.indices[offset + 2] * 3 + 0], mesh.tangents[mesh.indices[offset + 2] * 3 + 1], mesh.tangents[mesh.indices[offset + 2] * 3 + 2], 0));

			nrm[0] = glm::normalize(mat * glm::vec4(mesh.normals[mesh.indices[offset + 0] * 3 + 0], mesh.normals[mesh.indices[offset + 0] * 3 + 1], mesh.normals[mesh.indices[offset + 0] * 3 + 2], 0));
			nrm[1] = glm::normalize(mat * glm::vec4(mesh.normals[mesh.indices[offset + 1] * 3 + 0], mesh.normals[mesh.indices[offset + 1] * 3 + 1], mesh.normals[mesh.indices[offset + 1] * 3 + 2], 0));
			nrm[2] = glm::normalize(mat * glm::vec4(mesh.normals[mesh.indices[offset + 2] * 3 + 0], mesh.normals[mesh.indices[offset + 2] * 3 + 1], mesh.normals[mesh.indices[offset + 2] * 3 + 2], 0));

			pos[0] = mat * glm::vec4(mesh.position[mesh.indices[offset + 0] * 3 + 0], mesh.position[mesh.indices[offset + 0] * 3 + 1], mesh.position[mesh.indices[offset + 0] * 3 + 2], 0);
			pos[1] = mat * glm::vec4(mesh.position[mesh.indices[offset + 1] * 3 + 0], mesh.position[mesh.indices[offset + 1] * 3 + 1], mesh.position[mesh.indices[offset + 1] * 3 + 2], 0);
			pos[2] = mat * glm::vec4(mesh.position[mesh.indices[offset + 2] * 3 + 0], mesh.position[mesh.indices[offset + 2] * 3 + 1], mesh.position[mesh.indices[offset + 2] * 3 + 2], 0);

			for (int i = 0; i < 3; i++) {
				DrawLine(pos[i], pos[i] + (tan[i] * size), trans, COLOUR_RED);
				DrawLine(pos[i], pos[i] + (glm::cross(nrm[i], tan[i]) * size), trans, COLOUR_GREEN);
				DrawLine(pos[i], pos[i] + (nrm[i] * size), trans, COLOUR_BLUE);
			}

			offset += 3;
		}
	}

	void Gizmos::DrawRay(const Ray& ray, const glm::vec3& colour) {
		DrawLine(ray.GetOrigin(), ray.GetEnd(), glm::mat4(1), colour);
	}

	
	void Gizmos::DrawAABB(const BoundingBox& box, const glm::mat4& mat, const glm::vec3& colour)
	{
		glm::vec3 points[BE_BOUNDING_BOX_SIZE];

		box.GetAABB(mat, points);

		DrawConnectedLines({points[0], points[1], points[2], points[3]}, true, glm::mat4(1), colour);
		DrawConnectedLines({points[4], points[5], points[6], points[7]}, true, glm::mat4(1), colour);
		DrawLine({points[0], points[4], points[1], points[5], points[2], points[6], points[3], points[7]}, glm::mat4(1), colour);
	}

	void Gizmos::DrawOBB(const BoundingBox& box, const glm::mat4& mat, const glm::vec3& colour)
	{
		glm::vec3 points[BE_BOUNDING_BOX_SIZE];

		box.GetOBB(mat, points);

		DrawConnectedLines({ points[0], points[1], points[2], points[3] }, true, glm::mat4(1), colour);
		DrawConnectedLines({ points[4], points[5], points[6], points[7] }, true, glm::mat4(1), colour);
		DrawLine({ points[0], points[4], points[1], points[5], points[2], points[6], points[3], points[7] }, glm::mat4(1), colour);

		//glm::vec3 min = box.GetMin();
		//glm::vec3 max = box.GetMax();

		//DrawConnectedLines
		//(
		//	{
		//		glm::vec3(max.x, max.y, max.z),
		//		glm::vec3(min.x, max.y, max.z),
		//		glm::vec3(min.x, max.y, min.z),
		//		glm::vec3(max.x, max.y, min.z),
		//	},
		//	true, mat, colour
		//);
		//
		//DrawConnectedLines
		//(
		//	{
		//		glm::vec3(max.x, min.y, max.z),
		//		glm::vec3(min.x, min.y, max.z),
		//		glm::vec3(min.x, min.y, min.z),
		//		glm::vec3(max.x, min.y, min.z),
		//	},
		//	true, mat, colour
		//);
		//
		//DrawLine
		//(
		//	{
		//		glm::vec3(max.x, max.y, max.z),
		//		glm::vec3(max.x, min.y, max.z),
		//		glm::vec3(min.x, max.y, max.z),
		//		glm::vec3(min.x, min.y, max.z),
		//
		//		glm::vec3(min.x, max.y, min.z),
		//		glm::vec3(min.x, min.y, min.z),
		//		glm::vec3(max.x, max.y, min.z),
		//		glm::vec3(max.x, min.y, min.z),
		//	},
		//	mat, colour
		//);
	}
}