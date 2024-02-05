#pragma once
#include <vector>
#include "glm/mat4x4.hpp"

namespace BE {
	class BoundingBox;
	class MeshData
	{
	friend class Mesh;
	public:
		MeshData() {}
		MeshData(const MeshData& data) { Copy(data); }
		MeshData(const char* modelData, const size_t& length);
		~MeshData();

		std::vector<float> position;		// Vec3
		std::vector<float> normals;			// Vec3
		std::vector<float> uvs;				// Vec2
		std::vector<float> tangents;		// Vec3
		std::vector<float> colour0;			// Vec3
		std::vector<float> colour1;			// Vec3
		std::vector<float> colour2;			// Vec3
		// TODO: Compress boneIDs down into a single unsigned int, 1 byte per bone index
		std::vector<int> boneIds;			// iVec4
		std::vector<float> weights;			// Vec4

		std::vector<glm::mat4> boneOffsets;
		std::vector<unsigned int> indices{};

		//glm::mat4 transform = glm::mat4(1);

		void Clear();
		void Copy(const MeshData& mesh);
		void GenerateTangents();
		void GenerateNormals();

	protected:

		bool IsValid();
		void CreateBuffer(std::vector<float>& out_buffer, std::vector<unsigned int>& out_attributes, BoundingBox& bounds);

		/// <summary> DO NOT EDIT </summary>
		unsigned int vao = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int vbo = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int ebo = 0;
		/// <summary> DO NOT EDIT </summary>
		unsigned int indicesCount = 0;
	};
}

