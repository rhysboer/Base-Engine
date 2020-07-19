#include "Loader.h"

namespace BE {
	bool FileLoader::LoadOBJ(const char* path, std::vector<float>& vertexData, std::vector<unsigned int>& vertexAttributes) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> material;

		std::string error;
		std::string warning;

		if(!tinyobj::LoadObj(&attrib, &shapes, &material, &warning, &error, path)) {
			printf("ERROR: Object3D - Failed to load object at (%s)\n", path);
			return false;
		}

		if(!error.empty()) {
			printf("ERROR: Object3D - %s\n", error.c_str());
			return false;
		}

		//if(!warning.empty())
		//	printf("WARNING: Object3D - %s\n", warning.c_str());

		// Vertex Pos
		vertexAttributes.push_back(3);

		if(!attrib.normals.empty()) // Normal
			vertexAttributes.push_back(3);
		if(!attrib.colors.empty()) // Vertex Colour
			vertexAttributes.push_back(3);
		if(!attrib.texcoords.empty()) // Tex Coords
			vertexAttributes.push_back(2);

		for(size_t s = 0; s < 1; s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					// Vertex Position
					vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					vertexData.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

					// Vertex Normals
					if(!attrib.normals.empty()) {
						vertexData.push_back(attrib.normals[3 * idx.normal_index + 0]);
						vertexData.push_back(attrib.normals[3 * idx.normal_index + 1]);
						vertexData.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}

					// Vertex Color
					if(!attrib.colors.empty()) {
						vertexData.push_back(attrib.colors[3 * idx.vertex_index + 0]);
						vertexData.push_back(attrib.colors[3 * idx.vertex_index + 1]);
						vertexData.push_back(attrib.colors[3 * idx.vertex_index + 2]);
					}

					// Vertex Texcoords
					if(!attrib.texcoords.empty()) {
						vertexData.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						vertexData.push_back(1.0 - attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}

				index_offset += fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}
		}

		return true;
	}
}