#pragma once
#include <vector>
#include "Camera.h"
#include "Object3D.h"
#include "RayCast.h"

class Gizmos {
public:

	static void Init();
	static void Render();
	static void AddCamera(const glm::mat4& cameraMatrix);

	static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& colour = glm::vec3(0.5f, 0.5f, 0.5f));
	static void DrawRay(const RayCast::Ray& ray, const glm::vec3& colour = glm::vec3(0.5f, 0.5f, 0.5f));
	static void DrawCube(const glm::vec3& position, const unsigned int& size, const glm::vec3& colour = glm::vec3(0.5f, 0.5f, 0.5f));

private:

	static std::vector<Object3D*> gizmos;
	static glm::mat4 projectionView;
	static Shader* shader;
};

