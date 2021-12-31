#pragma once
#include <vector>
#include "Drawable.h"
#include "Camera.h"
#include "Loader.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Mesh.h"

#define SHADER_MODEL "model"
#define SHADER_PROJECTIONVIEW "projectionView"

namespace BE {
	class Object3D : public Drawable {

	// Static Functions
	public:

		// Primitive Shapes TODO: Move to its own file ObjectManager? 
		static Object3D* CreateCube(const glm::vec3& position);
		static Object3D* CreatePlane(const glm::vec3& position, const unsigned int& size);
		static Object3D* CreateSprite(const glm::vec3& position);
		static Object3D* CreateSphere(const glm::vec3& position, const float& diameter, const int& sectorCount, const int& stackCount);
		static Object3D* CreateFromOBJFile(const glm::vec3& position, const char* path);

	public:
		
		// TODO: Move to drawable??
		enum class RenderType {
			TRIANGLES = GL_TRIANGLES,
			LINES = GL_LINES
		};

		Object3D();
		Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<unsigned int>& vertex_data_attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
		~Object3D();


		// Inherited via Drawable
		virtual void Render(const glm::mat4& projectionView) override;
		virtual void Render(Camera& camera) override;
		virtual void RawRender() override;

		/* Setters */
		void SetRenderType(const RenderType& render);

		/* Getters */
		Shader* shader;
		Transform& GetTransform();
		inline Mesh* GetMesh() const { return mesh; }

	private:
		RenderType renderType;

		Transform* transform;
		Mesh* mesh;
	};
}