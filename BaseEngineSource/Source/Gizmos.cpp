#include "Gizmos.h"

namespace BaseEngine {
	std::vector<Object3D*> Gizmos::gizmos = std::vector<Object3D*>();
	glm::mat4 Gizmos::projectionView = glm::mat4(1);
	Shader* Gizmos::shader = nullptr;

	void Gizmos::Init() {
		printf("Debug Build: Gizmos Enabled!\n");

		shader = ShaderManager::AddShader("gizmo", "./data/shaders/default/gizmo");
	}

	void Gizmos::AddCamera(const glm::mat4& cameraMatrix) {
		projectionView = cameraMatrix;
	}

	void Gizmos::Render() {

		shader->SetMatrix4("projectionView", projectionView);

		for(int i = 0; i < gizmos.size(); i++) {
			shader->SetMatrix4("model", gizmos[i]->GetTransform().ModelMatrix());
			gizmos[i]->RawRender();

			delete gizmos[i];
		}

		gizmos.clear();
	}

	void Gizmos::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& colour) {
		std::vector<float> vertexData = std::vector<float>({
			start.x, start.y, start.z,	0.0f, 0.0f, 0.0f,	colour.x, colour.y, colour.z,	0.0f, 0.0f,
			end.x, end.y, end.z,		0.0f, 0.0f, 0.0f,	colour.x, colour.y, colour.z,	0.0f, 0.0f
														   });

		Object3D* obj = new Object3D(glm::vec3(0), vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
		obj->SetShader(shader);
		obj->SetRender(Object3D::RenderType::LINES);

		gizmos.push_back(obj);
	}

	void Gizmos::DrawCube(const glm::vec3& position, const float& size, const glm::vec3& colour) {
		std::vector<float> vertexData = std::vector<float>({
			// LEFT
			-1.0f * size, -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size, -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size, -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size,  1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size,  1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size,  1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size,  1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size, -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,

			// RIGHT				
			1.0f * size, -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size, -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size, -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size, -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,

			// BOT					
			-1.0f * size, -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  -1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size, -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  -1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,

			// TOP					
			-1.0f * size, 1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size,  1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			-1.0f * size, 1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
			1.0f * size,  1.0f * size, -1.0f * size,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,		0.0f, 0.0f,
														   });

		Object3D* obj = new Object3D(position, vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
		obj->SetShader(shader);
		obj->SetRender(Object3D::RenderType::LINES);

		gizmos.push_back(obj);
	}

	void Gizmos::DrawRay(const Raycast::Ray& ray, const glm::vec3& colour) {
		DrawLine(ray.origin, ray.origin + ray.direction * ray.distance, colour);
	}

	void Gizmos::DrawPlane(const Plane& plane, const glm::vec3& normalColour, const glm::vec3& colour) {
		glm::vec3 normal = plane.GetNormal();
		glm::vec3 pos = plane.GetPosition();

		float size = 5.0f;

		glm::vec3 dirA = plane.GetRight() * size;
		glm::vec3 dirB = glm::cross(plane.GetNormal(), plane.GetRight()) * size;

		std::vector<float> vertexData = std::vector<float>({
			// Direction 1
			dirA.x, dirA.y * size, dirA.z,			0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
			-dirA.x, -dirA.y * size, -dirA.z,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,

			// Direction 2
			dirB.x, dirB.y, dirB.z,			0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,
			-dirB.x, -dirB.y, -dirB.z,		0.0f, 0.0f, 0.0f,		colour.x, colour.y, colour.z,	0.0f, 0.0f,

			// Normal
			0.0f, 0.0f, 0.0f,					0.0f, 0.0f, 0.0f,		normalColour.x, normalColour.y, normalColour.z,		0.0f, 0.0f,
			normal.x, normal.y, normal.z,		0.0f, 0.0f, 0.0f,		normalColour.x, normalColour.y, normalColour.z,		0.0f, 0.0f,
														   });

		Object3D* obj = new Object3D(pos, vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
		obj->SetShader(shader);
		obj->SetRender(Object3D::RenderType::LINES);

		gizmos.push_back(obj);
	}

	void Gizmos::DrawAABB(const AABB& aabb, const glm::vec3& colour) {
		std::vector<float> vertexData = std::vector<float>();

		auto addData = [&](const glm::vec3& vert, const glm::vec3& norm, const glm::vec3& col, const glm::vec2& uv) {
			vertexData.push_back(vert.x);
			vertexData.push_back(vert.y);
			vertexData.push_back(vert.z);

			vertexData.push_back(norm.x);
			vertexData.push_back(norm.y);
			vertexData.push_back(norm.z);

			vertexData.push_back(col.x);
			vertexData.push_back(col.y);
			vertexData.push_back(col.z);

			vertexData.push_back(uv.x);
			vertexData.push_back(uv.y);
		};

		const unsigned int corners = 4;
		for(int i = 0; i < corners; i++) {
			// TOP
			glm::vec3 pointA = aabb.GetPoint(i);
			glm::vec3 pointB = aabb.GetPoint((i + 1) % corners);

			addData(pointA, glm::vec3(0), colour, glm::vec2(0));
			addData(pointB, glm::vec3(0), colour, glm::vec2(0));

			// BOT
			glm::vec3 pointC = aabb.GetPoint(i + corners);
			glm::vec3 pointD = aabb.GetPoint(((i + 1) % corners) + corners);

			addData(pointC, glm::vec3(0), colour, glm::vec2(0));
			addData(pointD, glm::vec3(0), colour, glm::vec2(0));

			// EDGES
			addData(pointA, glm::vec3(0), colour, glm::vec2(0));
			addData(pointC, glm::vec3(0), colour, glm::vec2(0));
		}


		Object3D* obj = new Object3D(aabb.GetOrigin(), vertexData, std::vector<unsigned int>({ 3, 3, 3, 2 }));
		obj->SetShader(shader);
		obj->SetRender(Object3D::RenderType::LINES);

		gizmos.push_back(obj);
	}
}