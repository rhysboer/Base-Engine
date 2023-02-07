#include "FLVERLoader.h"
#include "Logging.h"
#include <bitset>
#include "glm/glm.hpp"

#define FLVER_SIGNATURE "FLVER"

namespace BE::Loader {
	struct FLVERMesh;
	struct VertexGroup;

	struct MeshLoc {
		int start;
		int boneIndicesStartOffset;
	};

	struct MeshInfo {
		int materialIndex;
		std::vector<int> facesetIndices{};
		std::vector<int> vertexGroupIndices{};
	};

	struct VertexStructMember {
		int structOffset = 0;
		int memberValueType = -1;
		int semantic = -1;
		int index = 0;
	};

	struct VertexStructLayout {
		std::vector<VertexStructMember> members = std::vector<VertexStructMember>();
	};

	struct FLVERMesh {

		int isDynamic = 0; 
		int defaultBoneIndex = -1;

		std::vector<VertexGroup> vertexGroups;

		std::vector<float> vertexPos{};
		std::vector<float> vertexNorm{};
		std::vector<float> vertexBitang{};
		std::vector<float> vertexColor{};
		std::vector<float> vertexUVs{};

		// Unknown Vector4
		std::vector<int> boneIndices{};
		std::vector<float> boneWeights{};
	};


	struct VertexGroup {
	public:
		FLVERMesh* mesh;
		int vertexStructLayoutIndex;
		int vertexCount;
		int vertexSize;

		// Custom
		glm::ivec2 info;
	};

	int16_t ReadInt16(const std::string& data, size_t& offset);
	int32_t ReadInt32(const std::string& data, size_t& offset);
	float ReadFloat(const std::string& data, size_t& offset);
	char ReadByte(const std::string& data, size_t& offset);
	wchar_t ReadWChar(const std::string& data, size_t& offset);
	glm::vec3 ReadVec3(const std::string& data, size_t& offset);
	std::wstring ReadString(const std::string& data, size_t& offset);

	BE::Mesh* FLVER::Load(const std::string& data, const unsigned int& flags)
	{
		if (data.length() < 6) {
			BE_ERROR("FLVER::Load() : Missing FLVER header");
			return nullptr;
		}

		size_t offset = 0;
		bool l_endian = true;
		int version_major = 0;
		int version_minor = 0;

		// Check signature
		if (!(data.substr(0, 5) == FLVER_SIGNATURE)) {
			BE_ERROR("FLVER::Load() : File is not a proper FLVER file");
			return nullptr;
		}
		offset += 6;

		// Check Endianness
		switch (ReadWChar(data, offset))
		{
		case 'L': l_endian = true; break;
		case 'B': l_endian = false; break;
		default: BE_ERROR("FLVER::Load() : Failed to read file header"); return nullptr;
		}

		// Bypass version data
		offset+= 4;
		
		int dataOffset = ReadInt32(data, offset);
		int dataSize = ReadInt32(data, offset);
		int dummyCount = ReadInt32(data, offset);
		int materialCount = ReadInt32(data, offset);
		int boneCount = ReadInt32(data, offset);
		int meshCount = ReadInt32(data, offset);
		int vertexGroupCount = ReadInt32(data, offset); 

		// Bounding Box
		offset += 4 * 3;
		offset += 4 * 3;

		// Unknown values x4
		int unknown1 = ReadInt32(data, offset);
		int unknown2 = ReadInt32(data, offset);
		int unknown3 = ReadInt32(data, offset);
		int unknown4 = ReadInt32(data, offset);

		int faceCount = ReadInt32(data, offset);
		int vertexStructLayoutCount = ReadInt32(data, offset);
		int materialParameterCount = ReadInt32(data, offset);

		// Unknown Values x9 
		offset += 4 * 9;


		std::vector<MeshInfo> meshInfo{};
		std::vector<FLVERMesh> subMeshes{};

		// TODO: DUMMY COUNT

		// Material
		for (int i = 0; i < materialCount; i++) {
			size_t nameOffset = ReadInt32(data, offset);
			size_t mtdNameOffset = ReadInt32(data, offset);

			std::wstring name = ReadString(data, nameOffset);
			std::wstring MTDName = ReadString(data, mtdNameOffset);

			int paramCount = ReadInt32(data, offset);
			int paramStartIndex = ReadInt32(data, offset);

			int matFlags = ReadInt32(data, offset);
			// Unknown x3
			offset += 4 * 3;
		}

		// Bones
		for (int i = 0; i < boneCount; i++) {
			glm::vec3 translation = ReadVec3(data, offset);
			size_t nameOffset = ReadInt32(data, offset);

			std::wstring boneName = ReadString(data, nameOffset);

			glm::vec3 euler = ReadVec3(data, offset);

			int16_t parentIndex = ReadInt16(data, offset);
			int16_t childIndex = ReadInt16(data, offset);

			glm::vec3 scale = ReadVec3(data, offset);

			int16_t nextIndex = ReadInt16(data, offset);
			int16_t prevIndex = ReadInt16(data, offset);

			// TODO: Handle nulls for vectors
			glm::vec3 aabbMin = ReadVec3(data, offset);

			int16_t isNubValue = ReadInt16(data, offset);

			// Unknown int16
			offset += 2;

			glm::vec3 aabbMax = ReadVec3(data, offset);

			// Unknown int16 x2
			offset += 2 * 2;

			// Unknown int32 x12
			offset += 4 * 12;

			// Bone End
		}

		// Meshes
		for (int i = 0; i < meshCount; i++) {
			MeshInfo mInfo = MeshInfo();
			FLVERMesh fMesh = FLVERMesh();

			fMesh.isDynamic = ReadInt32(data, offset);

			mInfo.materialIndex = ReadInt32(data, offset);

			// Unknown Byte x8
			offset += 1 * 8;

			fMesh.defaultBoneIndex = ReadInt32(data, offset);
			int boneIndexCount = ReadInt32(data, offset);

			// Uknown Int32
			offset += 4;

			size_t boneIndicesOffset = ReadInt32(data, offset);
			for (int j = 0; j < boneIndexCount; j++) {
				fMesh.boneIndices.push_back(ReadInt32(data, boneIndicesOffset));
			}

			int faceSetIndicesCount = ReadInt32(data, offset);
			size_t faceSetIndicesOffset = ReadInt32(data, offset);

			for (int j = 0; j < faceSetIndicesCount; j++) {
				mInfo.facesetIndices.push_back(ReadInt32(data, faceSetIndicesOffset));
			}

			int vertexGroupIndicesCount = ReadInt32(data, offset);
			size_t vertexGroupIndicesOffset = ReadInt32(data, offset);

			for (int j = 0; j < vertexGroupCount; j++) {
				mInfo.vertexGroupIndices.push_back(ReadInt32(data, vertexGroupIndicesOffset));
			}

			meshInfo.push_back(mInfo);
			subMeshes.push_back(fMesh);
		}

		// Faces
		for (int i = 0; i < faceCount; i++) {
			unsigned int flags = (unsigned int)ReadInt32(data, offset);
			bool isTriangleStrip = ReadByte(data, offset);
			bool cullBackFaces = ReadByte(data, offset);
			// Unknown Byte x2
			offset += 1 * 2;

			int indexCount = ReadInt32(data, offset);
			int indexBufferOffset = ReadInt32(data, offset);
			int indexBufferSize = ReadInt32(data, offset);

			size_t faceDataOffset = indexBufferOffset + dataOffset;
			for (int j = 0; j < indexCount; j++) {
				uint16_t vertexIndices = ReadInt16(data, faceDataOffset);
			}

			// Unknown int32 x3
			offset += 4 * 3;
		}

		std::vector<VertexGroup> vertexGroups = std::vector<VertexGroup>();
		//std::vector<glm::ivec2> vertexGroupsInfo = std::vector<glm::ivec2>();

		// Vertex Groups
		for (int i = 0; i < vertexGroupCount; i++) {
			VertexGroup group = VertexGroup();

			// Unknown int32
			offset += 4 * 1;

			group.vertexStructLayoutIndex = ReadInt32(data, offset);
			group.vertexSize = ReadInt32(data, offset);

			group.vertexCount = ReadInt32(data, offset);

			// Unknown int32 x2
			offset += 4 * 2;

			int vertexBufferSize = ReadInt32(data, offset);
			int vertexBufferOffset = ReadInt32(data, offset);

			group.info = glm::ivec2(vertexBufferSize, vertexBufferOffset);
			
			vertexGroups.push_back(group);
			//vertexGroupsInfo.push_back(glm::ivec2(vertexBufferSize, vertexBufferOffset));
		}

		// Vertex Layout Count
		for (int i = 0; i < vertexStructLayoutCount; i++) {
			int memberCount = ReadInt32(data, offset);

			// Unknown int32 x2
			offset += 4 * 2;

			size_t memberOffset = ReadInt32(data, offset);
			for (int j = 0; j < memberCount; j++) {
				// Unknown int32
				memberOffset += 4 * 1;

				int structOffset = ReadInt32(data, memberOffset);
				int valueType = ReadInt32(data, memberOffset);
				int semantic = ReadInt32(data, memberOffset);
				int index = ReadInt32(data, memberOffset);

			}
		}

		// Mesh Count
		for (int i = 0; i < meshCount; i++) {
			for (int j = 0; j < meshInfo.size(); j++) {
				vertexGroups[meshInfo[i].vertexGroupIndices[i]].mesh = &subMeshes[i];
				subMeshes[i].vertexGroups.push_back(vertexGroups[meshInfo[i].vertexGroupIndices[i]]);
			}
		}

		// list_MaterialParams

		// Material Parameter Count
		for (int i = 0; i < materialParameterCount; i++) {
			size_t valueOffset = ReadInt32(data, offset);

			std::wstring value = ReadString(data, valueOffset);
		
			size_t nameOffset = ReadInt32(data, offset);

			std::wstring name = ReadString(data, nameOffset);

			// Unknown float x2
			offset += 4 * 2;

			// Unknown byte x4
			offset += 1 * 4;
			
			// Unknown int32 x3
			offset += 4 * 3;

			// ADD
		}

		// Material Count
		for (int i = 0; i < materialCount; i++) {
			// TODO
		}

		// Submeshes
		for (int i = 0; i < subMeshes.size(); i++) {
			for (int j = 0; j < subMeshes[i].vertexGroups.size(); j++) {
				VertexGroup* vertexGroup = &subMeshes[i].vertexGroups[j];

				glm::ivec2 vertexGroupInfo = vertexGroup->info;

				int vertexLayout = vertexGroup->vertexStructLayoutIndex;

				int verticesStartOffset = vertexGroupInfo.x * dataOffset;

				for (int k = 0; k < vertexGroup->vertexCount; k++) {
					int currentStartOffset = verticesStartOffset + k * vertexGroup->vertexSize;


				}
			}
		}

		return nullptr;
	}



	int16_t ReadInt16(const std::string& data, size_t& offset) {
		return ReadWChar(data, offset);
	}

	// TODO: Handle big endian
	int32_t ReadInt32(const std::string& data, size_t& offset) {
		int b = 0;
		for (int i = 3; i >= 0; i--)
			b = (b << 8) | (int)std::bitset<8>((unsigned char)data[offset + i]).to_ulong();

		offset += 4;
		return b;
	}

	float ReadFloat(const std::string& data, size_t& offset) {
		int v = ReadInt32(data, offset);
		return *(float*)&v;
	}

	char ReadByte(const std::string& data, size_t& offset) {
		offset += 1;
		return data[offset - 1];
	}

	wchar_t ReadWChar(const std::string& data, size_t& offset) {
		offset += 2;
		return (std::bitset<8>((unsigned char)data[offset - 1]).to_ulong() << 8) | std::bitset<8>((unsigned char)data[offset - 2]).to_ulong();
	}

	glm::vec3 ReadVec3(const std::string& data, size_t& offset) {
		

		return glm::vec3(
			ReadFloat(data, offset),
			ReadFloat(data, offset),
			ReadFloat(data, offset)
		);
	}

	std::wstring ReadString(const std::string& data, size_t& offset) {
		std::wstring s;
		size_t o = offset;
		while (true) {
			wchar_t c = ReadWChar(data, o);
			if(c == '\0')
				break;
			s += c;
		}

		return s;
	}
}

#undef FLVER_HEADER
