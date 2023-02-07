#include "Mesh.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Logging.h"
#include "glm/glm.hpp"
#include "BoundingBox.h"

namespace BE {
	Mesh::Mesh() : bounds(new BoundingBox()) { }

	Mesh::Mesh(const MeshDesc& meshDesc) : bounds(new BoundingBox()), descriptor(meshDesc) 
	{}

	Mesh::Mesh(const MeshData& meshData, const MeshDesc& meshDesc) : bounds(new BoundingBox()), descriptor(meshDesc)
	{
		CreateMesh(meshData);
	}

	Mesh::~Mesh() {
		Destroy();

		delete bounds;
	}

	unsigned int Mesh::CreateMesh()
	{
		meshes.push_back(new MeshData());
		return meshes.size() - 1;
	}

	void Mesh::CreateMesh(const MeshData& meshData)
	{
		meshes.push_back(new MeshData(meshData));
		Apply();
	}

	void Mesh::UpdateMesh(const MeshData& meshData, const unsigned int& index)
	{
		BE_ASSERT(index < meshes.size(), "Mesh::SetMeshData() - Out of range index %i", index);
		if (meshes[index] != nullptr) {
			MeshData* mesh = meshes[index];
			mesh->Copy(meshData);
		} else {
			meshes[index] = new MeshData(meshData);
		}
	}

	void Mesh::Apply() {
		for (int i = 0; i < meshes.size(); ++i) {
			MeshData* mesh = meshes[i];

			if (!mesh->IsValid())
				BE_ASSERT(false, "Mesh : Invalid MeshData at submesh index %i", i);

			if(mesh->vao == 0) {
				glGenVertexArrays(1, &mesh->vao);
				glGenBuffers(1, &mesh->vbo);

				if (descriptor.mapData) {
					glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * descriptor.maxMapSize, NULL, GL_STATIC_DRAW);
				}
			}
		
			std::vector<float> vertexBuffer{};
			std::vector<unsigned int> attributes{};
			mesh->CreateBuffer(vertexBuffer, attributes, *bounds);

			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

			if (!descriptor.mapData) {
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBuffer.size(), vertexBuffer.size() > 0 ? &vertexBuffer[0] : NULL, GL_STATIC_DRAW);
			} else {
				void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				memcpy(buffer, &vertexBuffer[0], sizeof(float) * glm::min<unsigned int>(vertexBuffer.size(), descriptor.maxMapSize));
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
		
			if(!mesh->indices.empty()) {
				if (mesh->ebo == 0) {
					glGenBuffers(1, &mesh->ebo);
				}
		
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), mesh->indices.size() > 0 ? &mesh->indices[0] : NULL, GL_STATIC_DRAW);
			}
		
			unsigned int attributesPerVertex = 0;
			for(int i = 0; i < attributes.size(); i++)
				attributesPerVertex += attributes[i];
		
			if (attributesPerVertex == 0)
				attributesPerVertex = 1;
		
			unsigned int offset = 0;
			for(int i = 0; i < attributes.size(); i++) {
				glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
				glEnableVertexAttribArray(i);
		
				offset += attributes[i];
			}
		
			mesh->indicesCount = (mesh->indices.empty()) ? mesh->position.size() : mesh->indices.size(); //(mesh->indices.empty()) ? (sizeof(float) * attributes.size()) / attributesPerVertex : mesh->indices.size();
		}
	}

	void Mesh::Clear(const unsigned int& subMeshIndex)
	{
		if (subMeshIndex >= meshes.size())
			return;
		
		MeshData* mesh = meshes[subMeshIndex];
		if (mesh != nullptr) {
			mesh->Clear();
		}

		bounds->Clear();
	}

	void Mesh::Destroy() {
		for (int i = 0; i < meshes.size(); i++) {
			MeshData* mesh = meshes[i];

			if (mesh != nullptr)
				delete mesh;
		}

		bounds->Clear();
		meshes.clear();
	}

	void Mesh::BindVAO(const unsigned int& subMesh) const {
		if (subMesh > meshes.size())
			return;
		
		glBindVertexArray(meshes[subMesh]->vao);
	}
}