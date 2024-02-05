#include "Chunk.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "glm/gtc/noise.hpp"

void Chunk::OnStart()
{
	auto meshRenderer = this->GetEntity()->GetComponent<BE::MeshRenderer>();

	BE::MeshData mesh = BE::MeshData();

	auto addVertex = [&](const float& x, const float& y, const float& z, const float& u, const float& v) {
		mesh.position.push_back(x); mesh.position.push_back(y); mesh.position.push_back(z);
		mesh.uvs.push_back(u); mesh.uvs.push_back(v);
	};

	const int size = 5;
	int index = 0;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
	
			float height0 = glm::perlin(glm::vec2(199 + (x + 0) * 0.5f, 272 + (y + 0) * 0.5f)) * 2.0f;
			float height1 = glm::perlin(glm::vec2(199 + (x + 1) * 0.5f, 272 + (y + 0) * 0.5f)) * 2.0f;
			float height2 = glm::perlin(glm::vec2(199 + (x + 0) * 0.5f, 272 + (y + 1) * 0.5f)) * 2.0f;
			float height3 = glm::perlin(glm::vec2(199 + (x + 1) * 0.5f, 272 + (y + 1) * 0.5f)) * 2.0f;
	
			addVertex(x    , height0, y    , 0, 0);
			addVertex(x    , height2, y + 1, 0, 1);
			addVertex(x + 1, height1, y    , 1, 0);
			addVertex(x + 1, height3, y + 1, 1, 1);
	
			mesh.indices.push_back(index + 0);
			mesh.indices.push_back(index + 1);
			mesh.indices.push_back(index + 2);
	
			mesh.indices.push_back(index + 1);
			mesh.indices.push_back(index + 3);
			mesh.indices.push_back(index + 2);
	
			index += 4;
		}
	}
	   
	mesh.GenerateNormals();
	mesh.GenerateTangents();
	meshRenderer->SetMesh(mesh);
}

void Chunk::OnProcess()
{
}

void Chunk::OnDestroy()
{
}

size_t Chunk::GetID() const
{
	return IComponent::GetComponentID<Chunk>();
}

const char* Chunk::GetName() const
{
	return "Chunk";
}
