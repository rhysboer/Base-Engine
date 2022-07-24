#include "OBJLoader.h"
#include "tiny_obj_loader.h"
#include "MeshLoader.h"
#include "BEMath.h"
#include <sstream>

namespace BE::Loader {
	BE::Mesh* OBJ::Load(const std::string& data, const unsigned int& flags) {

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> material;

		std::string error;
		std::string warning;

		std::istringstream s(data);
		if (!tinyobj::LoadObj(&attrib, &shapes, &material, &warning, &error, &s)) {
			// TODO: Log Errors
			return nullptr;
		}

		if (!error.empty()) {
			// TODO: Log Errors
			return nullptr;
		}

		const bool ignoreTangents = BE::Math::IsBitSet(flags, (int)MeshLoader::MeshFlags::IGNORE_TANGENT_SPACE - 1);

		BE::Mesh* mesh = new BE::Mesh();
		for (size_t s = 0; s < shapes.size(); s++) {
			unsigned int index = mesh->CreateSubMesh();
			SubMesh* subMesh = new SubMesh();

			// Position
			subMesh->attributes.push_back(3);
			// Normal
			if (!attrib.normals.empty())
				subMesh->attributes.push_back(3);
			// Tex Coords
			if (!attrib.texcoords.empty())
				subMesh->attributes.push_back(2);
			// Tangent Space
			if (!ignoreTangents)
				subMesh->attributes.push_back(3);
			// Vertex Colour
			//if (!attrib.colors.empty() && settings.importVertexColor)
			//	mesh->attributes.push_back(3);

			// Needed for tangent space calculations
			glm::vec3 vertex_pos[3];
			glm::vec2 vertex_uvs[3];

			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					// Vertex Position
					subMesh->vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 0] /* settings.importScale */);
					subMesh->vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 1] /* settings.importScale */);
					subMesh->vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 2] /* settings.importScale */);

					// Vertex Normals
					if (!attrib.normals.empty()) {
						subMesh->vertexData.push_back(attrib.normals[3 * idx.normal_index + 0]);
						subMesh->vertexData.push_back(attrib.normals[3 * idx.normal_index + 1]);
						subMesh->vertexData.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}

					// Vertex Color
					//if (!attrib.colors.empty() /* && settings.importVertexColor*/) {
					//	mesh->vertexData.push_back(attrib.colors[3 * idx.vertex_index + 0]);
					//	mesh->vertexData.push_back(attrib.colors[3 * idx.vertex_index + 1]);
					//	mesh->vertexData.push_back(attrib.colors[3 * idx.vertex_index + 2]);
					//}

					// UVs / Vertex Texcoords
					if (!attrib.texcoords.empty()) {
						subMesh->vertexData.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						subMesh->vertexData.push_back(1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]);
					}

					if (!ignoreTangents) {
						if ((v % 3) == 0) {
							for (int j = 0; j < 3; j++) {
								idx = shapes[s].mesh.indices[index_offset + v + j];

								vertex_pos[j].x = attrib.vertices[3 * idx.vertex_index + 0];
								vertex_pos[j].y = attrib.vertices[3 * idx.vertex_index + 1];
								vertex_pos[j].z = attrib.vertices[3 * idx.vertex_index + 2];

								vertex_uvs[j].x = attrib.texcoords[2 * idx.texcoord_index + 0];
								vertex_uvs[j].y = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];
							}
						}

						glm::vec3 e1 = vertex_pos[1] - vertex_pos[0];
						glm::vec3 e2 = vertex_pos[2] - vertex_pos[0];
						glm::vec2 uv1 = vertex_uvs[1] - vertex_uvs[0];
						glm::vec2 uv2 = vertex_uvs[2] - vertex_uvs[0];

						float f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

						// Tangent
						subMesh->vertexData.push_back(f * (uv2.y * e1.x - uv1.y * e2.x));
						subMesh->vertexData.push_back(f * (uv2.y * e1.y - uv1.y * e2.y));
						subMesh->vertexData.push_back(f * (uv2.y * e1.z - uv1.y * e2.z));
					}
				}

				index_offset += fv;

				// per-face material
				//shapes[s].mesh.material_ids[f];
			}

			mesh->SetMeshData(s, subMesh);
		}

		return mesh;
	}
}