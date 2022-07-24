#include "Mesh.h"
#include "Logging.h"

namespace BE {
	Mesh::Mesh() { }

	Mesh::Mesh(const Mesh const* mesh) {
		for (int i = 0; i < mesh->GetMeshCount(); i++) {
			unsigned int index = CreateSubMesh();
			SetMeshData(index, mesh->meshes[i]->vertexData, mesh->meshes[i]->attributes, mesh->meshes[i]->indices);
		}
	}

	Mesh::Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices) {
		int index = CreateSubMesh();
		SubMesh* mesh = meshes[index];
		
		// Generate Buffers
		glGenVertexArrays(1, &mesh->vao);
		glGenBuffers(1, &mesh->vbo);
		
		SetMeshData(0, vertexData, attributes, indices);
		Apply();
	}

	Mesh::~Mesh() {
		// Destroy();
	}

	void Mesh::SetMeshData(const unsigned int& subMeshIndex, const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices) {
		SubMesh* mesh = new SubMesh;
		mesh->vertexData = std::vector<float>(vertexData);
		mesh->attributes = std::vector<unsigned int>(attributes);
		mesh->indices = std::vector<unsigned int>(indices);

		SetMeshData(subMeshIndex, mesh);
	}

	void Mesh::SetMeshData(const unsigned int& subMeshIndex, SubMesh* subMesh)
	{
		BE_ASSERT(subMeshIndex < meshes.size(), "Mesh::SetMeshData() - Out of range submesh index %i", subMeshIndex);
		if (meshes[subMeshIndex] != nullptr) {
			SubMesh* mesh = meshes[subMeshIndex];
			mesh->Copy(subMesh);

			delete subMesh;
		} else {
			meshes[subMeshIndex] = subMesh;
		}
	}

	unsigned int Mesh::CreateSubMesh()
	{
		meshes.emplace_back(new SubMesh());
		return meshes.size() - 1;
	}

	void Mesh::Apply() {
		for (int i = 0; i < meshes.size(); ++i) {
			SubMesh* mesh = meshes[i];

			if(mesh->vao == 0) {
				glGenVertexArrays(1, &mesh->vao);
				glGenBuffers(1, &mesh->vbo);
			}
		
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), mesh->vertexData.size() > 0 ? &mesh->vertexData[0] : NULL, GL_STATIC_DRAW);
		
			if(!mesh->indices.empty()) {
				if(mesh->ebo == 0)
					glGenBuffers(1, &mesh->ebo);
		
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), mesh->indices.size() > 0 ? &mesh->indices[0] : NULL, GL_STATIC_DRAW);
			}
		
			unsigned int attributesPerVertex = 0;
			for(int i = 0; i < mesh->attributes.size(); i++)
				attributesPerVertex += mesh->attributes[i];
		
			if (attributesPerVertex == 0)
				attributesPerVertex = 1;
		
			unsigned int offset = 0;
			for(int i = 0; i < mesh->attributes.size(); i++) {
				glVertexAttribPointer(i, mesh->attributes[i], GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
				glEnableVertexAttribArray(i);
		
				offset += mesh->attributes[i];
			}
		
			mesh->indicesCount = (mesh->indices.empty()) ? (sizeof(float) * mesh->vertexData.size()) / attributesPerVertex : mesh->indices.size();
		}
	}

	//void Mesh::AddMeshData(const BE::Mesh* mesh, const unsigned int& s_subMeshIndex, const unsigned int& d_subMeshIndex, const glm::vec3& offset)
	//{
	//	if (s_subMeshIndex > mesh->meshes.size() || d_subMeshIndex > meshes.size()) {
	//		BE_ERROR("Mesh::AddMeshData() - Invalid subMesh index");
	//		return;
	//	}
	//	
	//	SubMesh* s_mesh = mesh->meshes[s_subMeshIndex];
	//	SubMesh* d_mesh = meshes[d_subMeshIndex];
	//	
	//	if (d_mesh->attributes.size() == 0)
	//		d_mesh->attributes = s_mesh->attributes;
	//	
	//	if (!std::equal(d_mesh->attributes.begin(), d_mesh->attributes.end(), s_mesh->attributes.begin(), s_mesh->attributes.end())) {
	//		BE_ERROR("Mesh::AddMeshData() - Mesh attributes don't match up.");
	//		return;
	//	}
	//	
	//	int start = d_mesh->vertexData.size();
	//	
	//	d_mesh->vertexData.insert(d_mesh->vertexData.end(), s_mesh->vertexData.begin(), s_mesh->vertexData.end());
	//	
	//	int end = d_mesh->vertexData.size();
	//	
	//	if (offset.x != 0 || offset.y != 0 || offset.z != 0) {
	//		unsigned int attributesPerVertex = 0;
	//		for (int i = 0; i < d_mesh->attributes.size(); i++)
	//			attributesPerVertex += d_mesh->attributes[i];
	//	
	//		for (int i = start; i < end; i += attributesPerVertex) {
	//			d_mesh->vertexData[i + 0] += offset.x;
	//			d_mesh->vertexData[i + 1] += offset.y;
	//			d_mesh->vertexData[i + 2] += offset.z;
	//		}
	//	}
	//}

	//void Mesh::TransformMesh(const glm::mat4& transform, std::vector<unsigned int> attributeIndexes)
	//{
	//	//int size = vertexData.size();
	//	//
	//	//if (size <= 0)
	//	//	return;
	//	//
	//	//unsigned int attributesPerVertex = 0;
	//	//for (int i = 0; i < this->attributes.size(); i++)
	//	//	attributesPerVertex += this->attributes[i];
	//	//
	//	//int index = 0;
	//	//int offset = 0;
	//	//glm::vec4 vertex = glm::vec4(1, 1, 1, 1);
	//	//for (int i = 0; i < size; i += attributesPerVertex) {
	//	//	offset = 0;
	//	//
	//	//	for (int k = 0; k < attributes.size(); k++) {
	//	//		if (index >= attributeIndexes.size())
	//	//			break;
	//	//
	//	//		if (k == attributeIndexes[index]) {
	//	//			index++;
	//	//
	//	//			for (int j = 0; j < attributes[k]; j++) {
	//	//				if (j >= 4)
	//	//					break;
	//	//
	//	//				vertex[j] = vertexData[i + j + offset];
	//	//			}
	//	//
	//	//			vertex = transform * vertex;
	//	//
	//	//			for (int j = 0; j < attributes[k]; j++) {
	//	//				vertexData[i + j + offset] = vertex[j];
	//	//			}
	//	//
	//	//			vertex.x = 1;
	//	//			vertex.y = 1;
	//	//			vertex.z = 1;
	//	//			vertex.w = 1;
	//	//		}
	//	//
	//	//		offset += attributes[k];
	//	//	}
	//	//
	//	//	index = 0;
	//	//}
	//}

	void Mesh::Clear(const unsigned int& subMeshIndex)
	{
		if (subMeshIndex >= meshes.size())
			return;

		SubMesh* mesh = meshes[subMeshIndex];
		if (mesh != nullptr) {
			mesh->vertexData.clear();
			mesh->attributes.clear();
			mesh->indices.clear();
		}
	}

	void Mesh::Destroy() {
		for (int i = 0; i < meshes.size(); i++) {
			SubMesh* mesh = meshes[i];

			if(mesh != nullptr && mesh->vao != 0) {
				glDeleteVertexArrays(1, &mesh->vao);
				glDeleteBuffers(1, &mesh->vbo);
		
				if(mesh->ebo != 0)
					glDeleteBuffers(1, &mesh->ebo);

				delete mesh;
			}
		}

		meshes.clear();
	}

	void Mesh::BindVAO(const unsigned int& subMesh) {
		if (subMesh > meshes.size())
			return;
		
		glBindVertexArray(meshes[subMesh]->vao);
	}

	void SubMesh::Copy(const SubMesh* const mesh)
	{
		this->attributes = std::vector<unsigned int>(mesh->attributes);
		this->indices = std::vector<unsigned int>(mesh->indices);
		this->vertexData = std::vector<float>(mesh->vertexData);
	}
}