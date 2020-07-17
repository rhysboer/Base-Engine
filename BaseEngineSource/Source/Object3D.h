#pragma once
#include <vector>
#include "Camera.h"
#include "Loader.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Mesh.h"

#define SHADER_MODEL "model"
#define SHADER_PROJECTIONVIEW "projectionView"

namespace BaseEngine {
	class Object3D {

	// Static Functions
	public:

		static Object3D* CreateCube(const glm::vec3& position);
		static Object3D* CreatePlane(const glm::vec3& position, const unsigned int& size);
		static Object3D* CreateQuad(const glm::vec3& position);
		// Todo
		static Object3D* CreateCircle(const glm::vec3& position, const float& radius);
		static Object3D* CreateFromOBJFile(const glm::vec3& position, const char* path);

	public:
		
		enum class RenderType {
			TRIANGLES = GL_TRIANGLES,
			LINES = GL_LINES
		};

		Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<unsigned int>& vertex_data_attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
		~Object3D();

		void Render(const glm::mat4& projectionView);
		void Render(Camera& camera);
		void RawRender();

		/* Setters */
		void SetShader(Shader* shader);
		void SetRender(const RenderType& render);

		/* Getters */
		Shader* GetShader() const;
		Transform& GetTransform();

	private:
		RenderType render;

		Transform* transform;
		Shader* shader;
		Mesh* mesh;
	};
}