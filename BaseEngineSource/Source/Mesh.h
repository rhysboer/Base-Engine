#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace BE {

	// TODO: Turn into a class, destructure destroy VAO is existant
	struct SubMesh {
		std::vector<float> vertexData{};
		std::vector<unsigned int> attributes{};
		std::vector<unsigned int> indices{};

		/// <summary> DO NOT EDIT </summary>
		unsigned int vao = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int vbo = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int ebo = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int indicesCount = 0;

		void Copy(const SubMesh* const mesh);
	};

	class Mesh {
	public:

		Mesh();
		Mesh(const Mesh const* mesh);
		Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
		~Mesh();

		void SetMeshData(const unsigned int& subMeshIndex, const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices = {});
		void SetMeshData(const unsigned int& subMeshIndex, SubMesh* subMesh);
		void Apply();

		unsigned int CreateSubMesh();

		// TODO: Work out these functions
		//void AddMeshData(const BE::Mesh* mesh, const unsigned int& s_subMeshIndex = 0, const unsigned int& d_subMeshIndex = 0, const glm::vec3& offset = { 0, 0, 0 });
		//void TransformMesh(const glm::mat4& transform, std::vector<unsigned int> attributeIndexes);
		
		void Clear(const unsigned int& subMeshIndex = 0);
		void Destroy();

		void BindVAO(const unsigned int& subMesh = 0);

		inline bool HasEBO(const unsigned int& subMesh = 0) { return (meshes[subMesh]->ebo > 0) ? true : false; }
		inline unsigned int GetIndicesCount(const unsigned int& subMesh = 0) { return subMesh < meshes.size() ? meshes[subMesh]->indicesCount : 0; }
		inline unsigned int GetMeshCount() const { return meshes.size(); }
		
	private:
		
		std::vector<SubMesh*> meshes = std::vector<SubMesh*>();
	};
}

