#include "MeshLoader.h"
#include "File.h"
#include "BEMath.h"
#include "glm/glm.hpp"
//#include "Parser.h"

#include "Logging.h"


//#define OBJ_HASH 3674688
//#define FLVER_HASH 1263813696
//#define FBX_HASH 1316288

namespace BE {
	BE::MeshData MeshLoader::LoadFile(const char* path, const unsigned int& flags)
	{
		auto file = File::LoadFile(path);
		if (file.empty())
			return BE::MeshData();
		
		return BE::MeshData(file.c_str(), file.length());

		//auto ext = File::GetFileExtension(path);
		//
		//unsigned long hash = 0;
		//for (int i = 0; i < ext.length(); i++) {
		//	hash = hash << 6;
		//
		//	char c = std::tolower(ext[i]);
		//
		//	if(!std::isalnum(c))
		//		continue;
		//
		//	c -= std::isdigit(c) ? 22 : 97;
		//
		//	hash = (hash | c);
		//	//BE_LOG("SHIFT: %s", std::bitset<64>(hash).to_string().c_str());
		//}


		//switch (hash)
		//{
		//case OBJ_HASH: return Parser::ParseOBJ(file, flags);
		//case FLVER_HASH: return Parser::ParseOBJ(file, flags);
		//case FBX_HASH: return Parser::ParseFBX(file, flags);
		//default:
		//	BE_ERROR("MeshLoader::LoadFile() - No mesh loader for %s files", ext.c_str());
		//	return nullptr;
		//}
	}

	MeshData MeshLoader::CreateCube(const float& size) {
		MeshData mesh = MeshData();

		mesh.position = {
			// Bot
		   -0.5f * size,-0.5f * size, 0.5f * size, // Top Left
		   -0.5f * size,-0.5f * size,-0.5f * size, // Bot Left
			0.5f * size,-0.5f * size,-0.5f * size, // Bot Right
			0.5f * size,-0.5f * size, 0.5f * size, // Top Right

			// Top
		   -0.5f * size, 0.5f * size,-0.5f * size, 
		   -0.5f * size, 0.5f * size, 0.5f * size, 
			0.5f * size, 0.5f * size, 0.5f * size, 
			0.5f * size, 0.5f * size,-0.5f * size, 

			// Left
		   -0.5f * size,-0.5f * size,-0.5f * size, 
		   -0.5f * size,-0.5f * size, 0.5f * size, 
		   -0.5f * size, 0.5f * size, 0.5f * size, 
		   -0.5f * size, 0.5f * size,-0.5f * size, 

		   // Right
		   0.5f * size,-0.5f * size, 0.5f * size, // bot left
		   0.5f * size,-0.5f * size,-0.5f * size, // bot right
		   0.5f * size, 0.5f * size,-0.5f * size, // top right
		   0.5f * size, 0.5f * size, 0.5f * size, // top left

		   // Front
		  -0.5f * size, 0.5f * size, 0.5f * size,  
		  -0.5f * size,-0.5f * size, 0.5f * size,  
		   0.5f * size,-0.5f * size, 0.5f * size,  
		   0.5f * size, 0.5f * size, 0.5f * size,  

		   // Back
		   0.5f * size, 0.5f * size,-0.5f * size,  // top left
		   0.5f * size,-0.5f * size,-0.5f * size,  // bot left
		  -0.5f * size,-0.5f * size,-0.5f * size,  // bot right
		  -0.5f * size, 0.5f * size,-0.5f * size,  // top right
		};
		mesh.normals = {
			 0.0f,-1.0f, 0.0f,
			 0.0f,-1.0f, 0.0f,
			 0.0f,-1.0f, 0.0f,
			 0.0f,-1.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,

			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			 1.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,

			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f,-1.0f,
			 0.0f, 0.0f,-1.0f,
			 0.0f, 0.0f,-1.0f,
			 0.0f, 0.0f,-1.0f
		};
		mesh.uvs = {
			 0.0f, 0.0f,
			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 1.0f, 0.0f,

			 0.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,

			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,

			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,

			 0.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 
			 0.0f, 1.0f,	  
			 0.0f, 0.0f,  
			 1.0f, 0.0f,  
			 1.0f, 1.0f,  
		};
		mesh.indices = {
			0, 1, 2,  0, 2, 3, // BOT
			4, 5, 6,  4, 6, 7, // TOP
			8, 9,10,  8,10,11, // LEFT
		   12,13,14, 12,14,15, // RIGHT
		   16,17,18, 16,18,19, // FRONT 16,17,18, 16,18,19,
		   20,21,22, 20,22,23  // BACK
		};
		mesh.GenerateTangents();

		return mesh;
	}

	MeshData MeshLoader::CreatePlane(const float& size) {
		return CreatePlaneOffset(0.0f, 0.0f, 0.0f, size);
	}

	MeshData MeshLoader::CreatePlaneOffset(const float& offsetX, const float& offsetY, const float& offsetZ, const float& size)
	{
		MeshData mesh = MeshData();

		mesh.position = {
			(-0.5f * size) + offsetX, 0.0f + offsetY ,(-0.5f * size) + offsetZ,
			(-0.5f * size) + offsetX, 0.0f + offsetY ,( 0.5f * size) + offsetZ,
			( 0.5f * size) + offsetX, 0.0f + offsetY ,( 0.5f * size) + offsetZ,
			( 0.5f * size) + offsetX, 0.0f + offsetY ,(-0.5f * size) + offsetZ,
		};
		mesh.normals = {
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		mesh.uvs = {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};
		mesh.indices = {
			0, 1, 2,
			0, 2, 3
		};
		mesh.GenerateTangents();

		return mesh;
	}

	MeshData MeshLoader::CreateQuad(const float& size) {
		return CreatePlane(size); // TODO: Change Plane
	}

	MeshData MeshLoader::CreateQuadOffset(const float& offsetX, const float& offsetY, const float& offsetZ, const float& size)
	{
		return CreatePlaneOffset(offsetX, offsetY, offsetZ, size);
	}

	MeshData MeshLoader::CreateSphere(const float& radius)
	{
		MeshData mesh = MeshData();

		glm::vec3 pos = glm::vec3(0, 0, 0);
		glm::vec3 norm = glm::vec3(0, 0, 0);
		glm::vec2 uvs = glm::vec2(0);

		auto AddData = [&]() {
			// Position
			mesh.position.push_back(pos.x);
			mesh.position.push_back(pos.y);
			mesh.position.push_back(pos.z);
			
			// Normal
			mesh.normals.push_back(norm.x);
			mesh.normals.push_back(norm.y);
			mesh.normals.push_back(norm.z);

			// UVs
			mesh.uvs.push_back(uvs.x);
			mesh.uvs.push_back(uvs.y);
		};

		const unsigned int sector = 31;
		const unsigned int stack = 29;

		const float sectorStep = (2.0f * BE_PI) / (float)sector;
		const float stackStep = BE_PI / (float)stack;

		const float radiusHalf = radius / 2.0f;

		// Stack
		for (int i = 0; i <= stack; i++) {
			float stackAngle = BE_PI / 2.0f - i * stackStep;

			// Sector
			for (int j = 0; j <= sector; j++) {
				float sectorAngle = j * sectorStep;

				pos.x = (radiusHalf * glm::cos(stackAngle)) * glm::cos(sectorAngle);
				pos.y = radiusHalf * glm::sin(stackAngle);
				pos.z = (radiusHalf * glm::cos(stackAngle)) * glm::sin(sectorAngle);

				norm = glm::normalize(pos);

				uvs.x = 1.0 - (float)j / (sector);
				uvs.y = 1.0 - (float)i / (stack);

				AddData();
			}
		}

		unsigned int p;
		int k1, k2;
		for (int i = 0; i < stack; i++) {
			k1 = i * (sector + 1);
			k2 = k1 + sector + 1;

			for (int j = 0; j < sector; j++, k1++, k2++) {
				if (i != 0)
				{
					mesh.indices.push_back(k1);
					mesh.indices.push_back(k1 + 1);
					mesh.indices.push_back(k2);
				}

				if (i != (stack - 1))
				{
					mesh.indices.push_back(k1 + 1);
					mesh.indices.push_back(k2 + 1);
					mesh.indices.push_back(k2);
				}
			}
		}

		mesh.GenerateTangents();
		return mesh;
	}
}

