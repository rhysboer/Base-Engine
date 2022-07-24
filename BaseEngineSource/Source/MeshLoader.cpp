#include "MeshLoader.h"
#include "File.h"
#include "BEMath.h"
#include "glm/glm.hpp"

#include "Logging.h"

#include "OBJLoader.h"
#include "FLVERLoader.h"

#define OBJ_HASH 3674688
#define FLVER_HASH 1263813696

namespace BE {
	Mesh* MeshLoader::LoadFile(const char* path, const unsigned int& flags)
	{
		auto file = File::LoadFile(path);
		auto ext = File::GetFileExtension(path);

		unsigned long hash = 0;
		for (int i = 0; i < ext.length(); i++) {
			hash = hash << 6;

			char c = std::tolower(ext[i]);

			if(!std::isalnum(c))
				continue;

			c -= std::isdigit(c) ? 22 : 97;

			hash = (hash | c);
			//BE_LOG("SHIFT: %s", std::bitset<64>(hash).to_string().c_str());
		}

		switch (hash)
		{
		case OBJ_HASH: return Loader::OBJ::Load(file, flags);
		case FLVER_HASH: return Loader::FLVER::Load(file, flags);
		default:
			BE_ERROR("MeshLoader::LoadFile() - No mesh loader for %s files", ext.c_str());
			return nullptr;
		}
	}

	Mesh* MeshLoader::CreateCube(const float& size) {
		return new Mesh(
			{
				// Bot
			   -0.5f * size,-0.5f * size, 0.5f * size,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,
			   -0.5f * size,-0.5f * size,-0.5f * size,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f,
				0.5f * size,-0.5f * size,-0.5f * size,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
				0.5f * size,-0.5f * size, 0.5f * size,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,

				// Top
			   -0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			   -0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

				// Left
			   -0.5f * size,-0.5f * size, 0.5f * size, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			   -0.5f * size,-0.5f * size,-0.5f * size, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			   -0.5f * size, 0.5f * size,-0.5f * size, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			   -0.5f * size, 0.5f * size, 0.5f * size, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

			   // Right
			   0.5f * size,-0.5f * size, 0.5f * size,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			   0.5f * size,-0.5f * size,-0.5f * size,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			   0.5f * size, 0.5f * size,-0.5f * size,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			   0.5f * size, 0.5f * size, 0.5f * size,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			   // Front
			  -0.5f * size,-0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			  -0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			   0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			   0.5f * size,-0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

			   // Back
			  -0.5f * size,-0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,
			  -0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
			   0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
			   0.5f * size,-0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f
			},
			{
				3, 3, 2
			},
			{
				0, 1, 2,  0, 2, 3, // BOT
				4, 5, 6,  4, 6, 7, // TOP
				8, 9,10,  8,10,11, // LEFT
			   12,13,14, 12,14,15, // RIGHT
			   16,17,18, 16,18,19, // FRONT
			   20,21,22, 20,22,23  // BACK
			}
			);
	}

	Mesh* MeshLoader::CreatePlane(const float& size) {
		return new Mesh(
			{
				// Bot
				-0.5f * size, 0.0f , 0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 1.0f,
				-0.5f * size, 0.0f ,-0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
				 0.5f * size, 0.0f ,-0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
				 0.5f * size, 0.0f , 0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,	0.0f, 0.0f, 1.0f
			},
			{
				// Position, Normal, UVs, Tangent
				3, 3, 2, 3
			},
			{
				0, 1, 2,  0, 2, 3
			}
			);
	}

	Mesh* MeshLoader::CreateQuad(const float& size) {
		return CreatePlane(size); // TODO: Change Plane
	}

	Mesh* MeshLoader::CreateSphere(const float& radius)
	{
		std::vector<float> vertexData = std::vector<float>();
		std::vector<unsigned int> indices = std::vector<unsigned int>();

		glm::vec3 pos = glm::vec3(0, 1, 0);
		glm::vec3 norm = glm::vec3(0, 1, 0);
		glm::vec2 uvs = glm::vec2(0);

		auto AddData = [&]() {
			// Position
			vertexData.push_back(pos.x);
			vertexData.push_back(pos.y);
			vertexData.push_back(pos.z);
			
			// Normal
			vertexData.push_back(norm.x);
			vertexData.push_back(norm.y);
			vertexData.push_back(norm.z);

			// UVs
			vertexData.push_back(uvs.x);
			vertexData.push_back(uvs.y);

			// Tangent
			vertexData.push_back(0);
			vertexData.push_back(0);
			vertexData.push_back(0);
		};

		const unsigned int sector = 32;
		const unsigned int stack = 29;

		const float sectorStep = (2.0f * BE_PI) / (float)sector;
		const float stackStep = BE_PI / (float)stack;

		// Stack
		for (int i = 0; i <= stack; i++) {
			float stackAngle = BE_PI / 2.0f - i * stackStep;

			// Sector
			for (int j = 0; j <= sector; j++) {
				float sectorAngle = j * sectorStep;

				pos.x = (radius * glm::cos(stackAngle)) * glm::cos(sectorAngle);
				pos.y = radius * glm::sin(stackAngle);
				pos.z = (radius * glm::cos(stackAngle)) * glm::sin(sectorAngle);

				norm = glm::normalize(pos);

				uvs.x = 1.0 - (float)j / (sector);
				uvs.y = 1.0 - (float)i / (stack);

				AddData();
			}
		}

		unsigned int p;
		int k1, k2;
		for (int i = 0; i <= stack; i++) {
			k1 = i * (sector + 1);
			k2 = k1 + sector + 1;

			for (int j = 0; j <= sector; j++, k1++, k2++) {
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (stack - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		Mesh* mesh = new BE::Mesh(vertexData, {3, 3, 2, 3}, indices);
		return mesh;
	}
}

