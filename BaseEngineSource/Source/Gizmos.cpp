#include "Gizmos.h"

std::vector<Object3D*> Gizmos::gizmos = std::vector<Object3D*>();
glm::mat4 Gizmos::projectionView = glm::mat4(1);
Shader* Gizmos::shader = nullptr;

void Gizmos::Init() {
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

	Object3D* obj = new Object3D(glm::vec3(0), vertexData, std::vector<unsigned int>({3, 3, 3, 2}));
	obj->SetShader(shader);
	obj->SetRender(Object3D::RenderType::LINES);

	gizmos.push_back(obj);
}

void Gizmos::DrawRay(const RayCast::Ray& ray, const glm::vec3& colour) {
	DrawLine(ray.origin, ray.origin + ray.direction * ray.distance, colour);
}

void Gizmos::DrawCube(const glm::vec3& position, const unsigned int& size, const glm::vec3& colour) {
	//std::vector<float> vertexData = std::vector<float>({
	//	-1.0f, -1.0f,  												   
	//});
}
