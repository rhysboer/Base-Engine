#include "Object3D.h"

namespace BE {

	Object3D::Object3D() : renderType(RenderType::TRIANGLES), shader(nullptr), mesh(new Mesh()), transform(new Transform()) { }

	Object3D::Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<unsigned int>& vertex_data_attributes, const std::vector<unsigned int>& indices)
		: renderType(RenderType::TRIANGLES), shader(nullptr) {

		mesh = new Mesh(vertex_data, vertex_data_attributes, indices);
		transform = new Transform();
		transform->SetPosition(position);
	}

	Object3D::~Object3D() {
		delete mesh;
		delete transform;
	}

	void Object3D::Render(Camera& camera) {
		Render(camera.GetProjectionView());
	}

	void Object3D::Render(const glm::mat4& projectionView) {
		if(!shader) return;

		mesh->BindVAO();

		shader->SetMatrix4(SHADER_MODEL, transform->ModelMatrix());
		shader->SetMatrix4(SHADER_PROJECTIONVIEW, projectionView);


		if(!mesh->HasEBO())
			glDrawArrays((GLenum)renderType, 0, mesh->GetIndicesCount());
		else
			glDrawElements((GLenum)renderType, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	}

	void Object3D::RawRender() {
		if(!shader) return;

		mesh->BindVAO();

		if(!mesh->HasEBO())
			glDrawArrays((GLenum)renderType, 0, mesh->GetIndicesCount());
		else
			glDrawElements((GLenum)renderType, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	}


	void Object3D::SetRenderType(const RenderType& render) {
		this->renderType = render;
	}

	Transform& Object3D::GetTransform() {
		return *transform;
	}


	///////////////////////////////////////////////////////////////
	// Create Objects 
	///////////////////////////////////////////////////////////////
#pragma region Primitives

	Object3D* Object3D::CreatePlane(const glm::vec3& position, const unsigned int& size) {
		std::vector<float> vertices = std::vector<float>();
		std::vector<unsigned int> attributes = std::vector<unsigned int>({ 3, 3, 2 });
		std::vector<unsigned int> indices = std::vector<unsigned int>();

		// Generate Vertices
		for(int y = 0; y <= size; y++) {
			for(int x = 0; x <= size; x++) {
				// Vertex
				vertices.push_back((x / (float)size) * 2.0 - 1.0);	// X
				vertices.push_back(0);								// Y
				vertices.push_back((y / (float)size) * 2.0 - 1.0);	// Z

				// Normals
				vertices.push_back(0.0f);
				vertices.push_back(1.0f);
				vertices.push_back(0.0f);

				// Texture Coorindates
				vertices.push_back(x / (float)size);
				vertices.push_back(y / (float)size);
			}
		}

		// Generate Indices
		const int sizeOffset = size + 1;
		for(int y = 0; y < size; y++) {
			for(int x = 0; x < size; x++) {
				indices.push_back(x + (y * sizeOffset));
				indices.push_back(x + (y * sizeOffset) + 1);
				indices.push_back(((y + 1) * sizeOffset) + x);

				indices.push_back(x + (y * sizeOffset) + 1);
				indices.push_back(((y + 1) * sizeOffset) + x + 1);
				indices.push_back(((y + 1) * sizeOffset) + x);
			}
		}

		return new Object3D(position, vertices, attributes, indices);
	}

	Object3D* Object3D::CreateCube(const glm::vec3& position) {
		std::vector<float> vertices = std::vector<float>({
			/* 1-3 = Vertex Pos, 4-6 = Normals, 7-8 = Texture Coords*/
			/* FRONT */
			 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
			 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
			-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 2

			-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 3
			-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 2
			 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1

			/* RIGHT */
			 1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			 /* BACK */
			-1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f,
			 1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f,

			 1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
			 1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f,

			/* LEFT */
			-1.0f,-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			-1.0f, 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			/* TOP */
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			 /* BOT */
			-1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f,

			 1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,-1.0f,-1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
														 });
		// Attributes: Vertex Position, Normals Texture Coordinates
		std::vector<unsigned int> attributes = std::vector<unsigned int>({
			3, 3, 2
		});

		return new Object3D(position, vertices, attributes);
	}

	Object3D* Object3D::CreateSprite(const glm::vec3& position) {
		std::vector<float> vertices = std::vector<float>({
			/* 1-3 = Vertex Pos, 4-6 = Normals, 7-8 = Texture Coords*/
			-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

			 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
														 });
		// Attributes: Vertex Position, Texture Coordinates
		std::vector<unsigned int> attributes = std::vector<unsigned int>({
			3, 3, 2
																		 });

		return new Object3D(position, vertices, attributes);
	}

	Object3D* Object3D::CreateSphere(const glm::vec3& position, const float& diameter, const int& sectorCount, const int& stackCount) {
		std::vector<float> vertexData = std::vector<float>();
		std::vector<unsigned int> indices = std::vector<unsigned int>();

		auto AddData = [&](const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& uv) {
			vertexData.push_back(vert.x);
			vertexData.push_back(vert.y);
			vertexData.push_back(vert.z);

			vertexData.push_back(norm.x);
			vertexData.push_back(norm.y);
			vertexData.push_back(norm.z);

			vertexData.push_back(uv.x);
			vertexData.push_back(uv.y);
		};

		const float radius = diameter / 2.0f;
		const float pi = glm::pi<float>();

		// Top
		AddData(glm::vec3(0, radius, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0));

		for(int sector = 0; sector < sectorCount; sector++) {
			// Horizontal Line
			float hoz = (2.0f * pi) * ((float)sector / sectorCount);

			for(int stack = 1; stack < stackCount; stack++) {

				// Verticle Line
				float ver = (pi / 2.0f) - pi * ((float)stack / stackCount);

				glm::vec3 pos = glm::vec3(
					(radius * glm::cos(ver)) * glm::cos(hoz),
					radius * glm::sin(ver),
					(radius * glm::cos(ver)) * glm::sin(hoz)
				);

				AddData(pos, glm::normalize(pos), glm::vec2(0));
			}
		}

		// Bottom
		AddData(glm::vec3(0, -radius, 0), glm::vec3(0, -1, 0), glm::vec2(0, 0));

		const int max = (stackCount - 1) * sectorCount + 1;
		for(int j = 0; j < sectorCount; j++) {
			int offset = (stackCount - 1) * j;
			int offset2 = (j == sectorCount - 1) ? -(stackCount - 1) : offset;

			for(int i = 0; i < stackCount; i++) {
				if(i == 0) {
					indices.push_back(0);
					indices.push_back(1 + offset);
					indices.push_back(stackCount + offset2);
					continue;
				} else if(i == stackCount - 1) {
					indices.push_back(max);
					indices.push_back(stackCount + i - 1 + offset2);
					indices.push_back(i + offset);
					continue;
				}

				indices.push_back(i + offset);
				indices.push_back(i + 1 + offset);
				indices.push_back(stackCount + i - 1 + offset2);

				indices.push_back(stackCount + i - 1 + offset2);
				indices.push_back(i + 1 + offset);
				indices.push_back(stackCount + i + offset2);
			}
		} 

		return new Object3D(position, vertexData, std::vector<unsigned int>({ 3, 3, 2 }), indices);
	}

	Object3D* Object3D::CreateFromOBJFile(const glm::vec3& position, const char* path) {
		std::vector<float> vertexData = std::vector<float>();
		std::vector<unsigned int> vertexAttributes = std::vector<unsigned int>();

		//if(FileLoader::LoadOBJ(path, vertexData, vertexAttributes))
		//	return new Object3D(position, vertexData, vertexAttributes);
		//else
			return nullptr;
	}

#pragma endregion
}