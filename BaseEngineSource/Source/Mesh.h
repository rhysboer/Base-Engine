#pragma once
#include <vector>
#include <string>
#include "MeshData.h"

namespace BE {
	struct MeshDesc {

		bool mapData = false;
		unsigned int maxMapSize = 0;
	};

	class BoundingBox;
	class Mesh {
	public:

		Mesh();
		Mesh(const MeshDesc& meshDesc);
		Mesh(const MeshData& meshData, const MeshDesc& meshDesc = MeshDesc());
		Mesh(const Mesh&) = delete; // TODO
		~Mesh();

		unsigned int CreateMesh();
		void CreateMesh(const MeshData& meshData);
		void UpdateMesh(const MeshData& meshData, const unsigned int& index = 0);
		
		void Apply();
		void Clear(const unsigned int& subMeshIndex = 0);
		void Destroy();

		void BindVAO(const unsigned int& subMesh = 0) const;

		inline bool HasEBO(const unsigned int& subMesh = 0) { return (meshes[subMesh]->ebo > 0) ? true : false; }
		inline unsigned int GetIndicesCount(const unsigned int& subMesh = 0) { return subMesh < meshes.size() ? meshes[subMesh]->indicesCount : 0; }
		inline unsigned int GetMeshCount() const { return meshes.size(); }
		inline const BoundingBox* const GetBounds() const { return bounds; }

	protected:
		
		MeshDesc descriptor;
		BoundingBox* bounds;
		std::vector<MeshData*> meshes = std::vector<MeshData*>();
	};
}

