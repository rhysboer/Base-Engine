#pragma once
#include <vector>
#include "BaseCamera.h"
#include "tiny_obj_loader.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Mesh.h"

#define SHADER_MODEL "model"
#define SHADER_PROJECTIONVIEW "projectionView"

class Object3D {
public:
	enum class RenderType {
		TRIANGLES = GL_TRIANGLES,
		LINES = GL_LINES
	};

	Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<unsigned int>& vertex_data_attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
	~Object3D();

	void Render(const glm::mat4& projectionView);
	void Render(BaseCamera& camera);
	void RawRender();

	/* Setters */
	void SetShader(Shader* shader);
	void SetRender(const RenderType& render);
	
	/* Getters */
	Shader* GetShader() const;
	Transform& GetTransform();

#pragma region Object Creation

	static Object3D* CreateObject_Plane(const glm::vec3& position, const unsigned int& size);
	static Object3D* CreateObject_Cube(const glm::vec3& position);
	static Object3D* CreateObject_Quad(const glm::vec3& position);
	static Object3D* CreateObject_FromFile(const glm::vec3& position, const char* path);

#pragma endregion

private:
	RenderType render;

	Transform* transform;
	Shader* shader;
	Mesh* mesh;
};
