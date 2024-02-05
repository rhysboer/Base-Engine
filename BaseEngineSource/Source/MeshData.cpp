#include "MeshData.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "BoundingBox.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "Logging.h"
#include <glm/ext/matrix_transform.hpp>

namespace BE {
	MeshData::MeshData(const char* modelData, const size_t& length)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(modelData, length, 0);
		glm::mat4 transform;

		auto addData = [&](float& x, float& y, float& z, std::vector<float>& list) {
			static glm::vec4 v = glm::vec4(1);
			v.x = x;
			v.y = y;
			v.z = z;
			v.w = 1;

			v = transform * v;

			list.emplace_back(v.x);
			list.emplace_back(v.y);
			list.emplace_back(v.z);
		};

		auto convertMatrix = [=](aiMatrix4x4* mat) {
			return glm::mat4(
				mat->a1, mat->b1, mat->c1, mat->d1,
				mat->a2, mat->b2, mat->c2, mat->d2,
				mat->a3, mat->b3, mat->c3, mat->d3,
				mat->a4, mat->b4, mat->c4, mat->d4
			);
		};

		// TODO: Support multiple meshes
		for (int m = 0; m < /*scene->mNumMeshes*/ 1; m++) {
			auto mesh = scene->mMeshes[m];

			const int vertexCount = mesh->mNumVertices * 3;
			position.reserve(vertexCount);
			normals.reserve(vertexCount);
			uvs.reserve(mesh->mNumVertices * 2);

			transform = /*glm::inverse*/(convertMatrix(&scene->mRootNode->mTransformation));

			for (int v = 0; v < mesh->mNumVertices; v++) {
				addData( mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, position );
				addData(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, normals);

				uvs.emplace_back(mesh->mTextureCoords[0][v].x);
				uvs.emplace_back(mesh->mTextureCoords[0][v].y);
			}

			for (int f = 0; f < mesh->mNumFaces; f++) {
				indices.emplace_back(mesh->mFaces[f].mIndices[0]);
				indices.emplace_back(mesh->mFaces[f].mIndices[1]);
				indices.emplace_back(mesh->mFaces[f].mIndices[2]);

				if (mesh->mFaces->mNumIndices > 3) {
					indices.emplace_back(mesh->mFaces[f].mIndices[0]);
					indices.emplace_back(mesh->mFaces[f].mIndices[2]);
					indices.emplace_back(mesh->mFaces[f].mIndices[3]);
				}
			}

			// Bones
			const int MAX_BONES_PER_VERTEX = 4;

			boneIds = std::vector<int>(mesh->mNumVertices * MAX_BONES_PER_VERTEX, -1);
			weights = std::vector<float>(mesh->mNumVertices * MAX_BONES_PER_VERTEX, 0.0f);

			for (int b = 0; b < mesh->mNumBones; b++) {
				auto offset = mesh->mBones[b]->mOffsetMatrix;
				boneOffsets.emplace_back(convertMatrix(&offset)); // transform * glm::inverse(convertMatrix(&offset)));
				//boneOffsets.emplace_back(/*glm::inverse*/(glm::mat4(
				//	offset.a1, offset.b1, offset.c1, offset.d1,
				//	offset.a2, offset.b2, offset.c2, offset.d2,
				//	offset.a3, offset.b3, offset.c3, offset.d3,
				//	offset.a4, offset.b4, offset.c4, offset.d4
				//)));
				//boneOffsets.emplace_back(glm::translate(glm::mat4(1), glm::vec3(b * 0.5f, 0, 0)));

				for (int w = 0; w < mesh->mBones[b]->mNumWeights; w++) {
					//VertexWeight weight{};

					int index = mesh->mBones[b]->mWeights[w].mVertexId * MAX_BONES_PER_VERTEX;
					for (int i = 0; i < MAX_BONES_PER_VERTEX; i++) {
						if (boneIds[index + i] == -1) {
							boneIds[index + i] = b;
							weights[index + i] = mesh->mBones[b]->mWeights[w].mWeight;
							break;
						}
					}
				}
			}
		}

			GenerateTangents();
	}

	MeshData::~MeshData()
	{
		if (vbo != 0)
			glDeleteBuffers(1, &vbo);
		if (ebo != 0)
			glDeleteBuffers(1, &ebo);
		if (vao != 0)
			glDeleteBuffers(1, &vao);
	}

	void MeshData::Clear()
	{
		position.clear();
		normals.clear();
		colour0.clear();
		colour1.clear();
		colour2.clear();
		uvs.clear();
		tangents.clear();
		indices.clear();
	}

	void MeshData::Copy(const MeshData& mesh)
	{
		position = std::vector<float>(mesh.position);
		normals = std::vector<float>(mesh.normals);
		colour0 = std::vector<float>(mesh.colour0);
		colour1 = std::vector<float>(mesh.colour1);
		colour2 = std::vector<float>(mesh.colour2);
		uvs = std::vector<float>(mesh.uvs);
		tangents = std::vector<float>(mesh.tangents);
		indices = std::vector<unsigned int>(mesh.indices);
		boneIds = std::vector<int>(mesh.boneIds);
		weights = std::vector<float>(mesh.weights);
		boneOffsets = std::vector<glm::mat4>(mesh.boneOffsets);

		//transform = mesh.transform;
	}

	void MeshData::GenerateTangents()
	{
		unsigned int offset = 0;

		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;

		glm::vec2 uv1;
		glm::vec2 uv2;
		glm::vec2 uv3;

		tangents.clear();
		tangents.resize(position.size(), 0);

		// TODO: Handle without indices
		while (offset < indices.size()) {
			pos1 = glm::vec3(position[indices[offset + 0] * 3 + 0], position[indices[offset + 0] * 3 + 1], position[indices[offset + 0] * 3 + 2]);
			pos2 = glm::vec3(position[indices[offset + 1] * 3 + 0], position[indices[offset + 1] * 3 + 1], position[indices[offset + 1] * 3 + 2]);
			pos3 = glm::vec3(position[indices[offset + 2] * 3 + 0], position[indices[offset + 2] * 3 + 1], position[indices[offset + 2] * 3 + 2]);

			uv1 = glm::vec2(uvs[indices[offset + 0] * 2 + 0], uvs[indices[offset + 0] * 2 + 1]);
			uv2 = glm::vec2(uvs[indices[offset + 1] * 2 + 0], uvs[indices[offset + 1] * 2 + 1]);
			uv3 = glm::vec2(uvs[indices[offset + 2] * 2 + 0], uvs[indices[offset + 2] * 2 + 1]);

			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;

			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			for (int i = 0; i < 3; i++) {
				tangents[indices[offset + i] * 3 + 0] = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangents[indices[offset + i] * 3 + 1] = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangents[indices[offset + i] * 3 + 2] = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			}

			offset += 3;
		}
	}

	void MeshData::GenerateNormals()
	{
		const bool hasIndices = indices.size() > 0;
		unsigned int offset = 0;

		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;

		glm::vec3 edge1;
		glm::vec3 edge2;

		normals.clear();
		normals.resize(position.size(), 0);

		// TODO: Handle without indices
		while (offset < indices.size()) {
			pos1 = glm::vec3(position[indices[offset + 0] * 3 + 0], position[indices[offset + 0] * 3 + 1], position[indices[offset + 0] * 3 + 2]);
			pos2 = glm::vec3(position[indices[offset + 1] * 3 + 0], position[indices[offset + 1] * 3 + 1], position[indices[offset + 1] * 3 + 2]);
			pos3 = glm::vec3(position[indices[offset + 2] * 3 + 0], position[indices[offset + 2] * 3 + 1], position[indices[offset + 2] * 3 + 2]);

			edge1 = pos2 - pos1;
			edge2 = pos3 - pos1;

			for (int i = 0; i < 3; i++) {
				normals[indices[offset + i] * 3 + 0] = (edge1.y * edge2.z) - (edge1.z * edge2.y);
				normals[indices[offset + i] * 3 + 1] = (edge1.z * edge2.x) - (edge1.x * edge2.z);
				normals[indices[offset + i] * 3 + 2] = (edge1.x * edge2.y) - (edge1.y * edge2.x);
			}

			offset += 3;
		}
	}

	bool MeshData::IsValid()
	{
		bool isValid = true;

		if (normals.size() > 0 && normals.size() != position.size()) {
			BE_ERROR("MeshData : Invalid amount of data in 'Normals' %i", normals.size());
			isValid = false;
		}

		// TODO
		//if (uvs.size() != position.size() * 0.6f) {
		//	BE_ERROR("MeshData : Invalid amount of data in 'Normal0' %i", normals0.size());
		//	isValid = false;
		//}

		if (tangents.size() > 0 && tangents.size() != position.size()) {
			BE_ERROR("MeshData : Invalid amount of data in 'Tangets' %i", tangents.size());
			isValid = false;
		}

		if (colour0.size() > 0 && colour0.size() != position.size()) {
			BE_ERROR("MeshData : Invalid amount of data in 'Colour0' %i", colour0.size());
			isValid = false;
		}

		if (colour1.size() > 0 && colour1.size() != position.size()) {
			BE_ERROR("MeshData : Invalid amount of data in 'Colour1' %i", colour1.size());
			isValid = false;
		}

		if (colour2.size() > 0 && colour2.size() != position.size()) {
			BE_ERROR("MeshData : Invalid amount of data in 'Colour2' %i", colour2.size());
			isValid = false;
		}

		return isValid;
	}

	void MeshData::CreateBuffer(std::vector<float>& out_buffer, std::vector<unsigned int>& out_attributes, BoundingBox& bounds)
	{
		unsigned int vec4_index = 0;
		unsigned int vec3_index = 0;
		unsigned int vec2_index = 0;

		const bool hasNormals = normals.size() > 0;
		const bool hasColours0 = colour0.size() > 0;
		const bool hasColours1 = colour1.size() > 0;
		const bool hasColours2 = colour2.size() > 0;
		const bool hasTangents = tangents.size() > 0;
		const bool hasUVs = uvs.size() > 0;
		const bool hasBones = boneOffsets.size() > 0;

		out_attributes.emplace_back(3); // Position
		if (hasNormals)
			out_attributes.emplace_back(3); // Normal
		if (hasUVs)
			out_attributes.emplace_back(2); // UVs
		if (hasTangents)
			out_attributes.emplace_back(3); // Tangents
		if (hasColours0)
			out_attributes.emplace_back(3); // Colour0
		if (hasColours1)
			out_attributes.emplace_back(3); // Colour1
		if (hasColours2)
			out_attributes.emplace_back(3); // Colour2

		if (hasBones) {
			out_attributes.emplace_back(4); // Vertex Ids
			out_attributes.emplace_back(4); // Weights
		}


		while (vec3_index < position.size()) {
			// 0 - Position
			out_buffer.emplace_back(position[vec3_index + 0]);
			out_buffer.emplace_back(position[vec3_index + 1]);
			out_buffer.emplace_back(position[vec3_index + 2]);

			bounds.Create(
				glm::min({ position[vec3_index + 0], position[vec3_index + 1], position[vec3_index + 2] }, bounds.GetMin()),
				glm::max({ position[vec3_index + 0], position[vec3_index + 1], position[vec3_index + 2] }, bounds.GetMax())
			);

			// 1 - Normal
			if (hasNormals) {
				out_buffer.emplace_back(normals[vec3_index + 0]);
				out_buffer.emplace_back(normals[vec3_index + 1]);
				out_buffer.emplace_back(normals[vec3_index + 2]);
			}

			// 2 - UVs
			if (hasUVs) {
				out_buffer.emplace_back(uvs[vec2_index + 0]);
				out_buffer.emplace_back(uvs[vec2_index + 1]);
			}

			// 3 - Tangent
			if (hasTangents) {
				out_buffer.emplace_back(tangents[vec3_index + 0]);
				out_buffer.emplace_back(tangents[vec3_index + 1]);
				out_buffer.emplace_back(tangents[vec3_index + 2]);
			}

			// 4 - Colour0
			if (hasColours0) {
				out_buffer.emplace_back(colour0[vec3_index + 0]);
				out_buffer.emplace_back(colour0[vec3_index + 1]);
				out_buffer.emplace_back(colour0[vec3_index + 2]);
			}

			// 5 - Colour1
			if (hasColours1) {
				out_buffer.emplace_back(colour1[vec3_index + 0]);
				out_buffer.emplace_back(colour1[vec3_index + 1]);
				out_buffer.emplace_back(colour1[vec3_index + 2]);
			}

			// 6 - Colour2
			if (hasColours2) {
				out_buffer.emplace_back(colour2[vec3_index + 0]);
				out_buffer.emplace_back(colour2[vec3_index + 1]);
				out_buffer.emplace_back(colour2[vec3_index + 2]);
			}

			if (hasBones) {
				// 7 - Bone IDs
				out_buffer.emplace_back(boneIds[vec4_index + 0]);
				out_buffer.emplace_back(boneIds[vec4_index + 1]);
				out_buffer.emplace_back(boneIds[vec4_index + 2]);
				out_buffer.emplace_back(boneIds[vec4_index + 3]);

				// 8 - Weights
				out_buffer.emplace_back(weights[vec4_index + 0]);
				out_buffer.emplace_back(weights[vec4_index + 1]);
				out_buffer.emplace_back(weights[vec4_index + 2]);
				out_buffer.emplace_back(weights[vec4_index + 3]);
			}

			vec4_index += 4;
			vec3_index += 3;
			vec2_index += 2;
		}
	}
}