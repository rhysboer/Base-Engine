#include "Object3D.h"

namespace BE {
	Object3D::Object3D(const glm::vec3& position, const std::vector<float>& vertex_data, const std::vector<unsigned int>& vertex_data_attributes, const std::vector<unsigned int>& indices)
		: render(RenderType::TRIANGLES), shader(nullptr) {

		mesh = new Mesh(vertex_data, vertex_data_attributes, indices);
		transform = new Transform();
		transform->SetPosition(position);
	}

	Object3D::~Object3D() {
		delete mesh;
		delete transform;
	}

	void Object3D::Render(Camera& camera) {
		Render(camera.ProjectionView());
	}

	void Object3D::Render(const glm::mat4& projectionView) {
		if(!shader) return;

		mesh->BindVAO();

		shader->SetMatrix4(SHADER_MODEL, transform->ModelMatrix());
		shader->SetMatrix4(SHADER_PROJECTIONVIEW, projectionView);

		if(render == RenderType::TRIANGLES) {
			if(!mesh->UsingEBO())
				glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount());
			else
				glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		} else if(render == RenderType::LINES) {
			glDrawArrays(GL_LINES, 0, mesh->GetIndicesCount());
		}
	}

	void Object3D::RawRender() {
		if(!shader) return;

		mesh->BindVAO();

		if(render == RenderType::TRIANGLES) {
			if(!mesh->UsingEBO())
				glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount());
			else
				glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		} else if(render == RenderType::LINES) {
			glDrawArrays(GL_LINES, 0, mesh->GetIndicesCount());
		}
	}

	void Object3D::SetShader(Shader* shader) {
		this->shader = shader;
	}

	void Object3D::SetRender(const RenderType& render) {
		this->render = render;
	}

	Shader* Object3D::GetShader() const {
		return shader;
	}

	Transform& Object3D::GetTransform() {
		return *transform;
	}


	// Create Objects 
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

	Object3D* Object3D::CreateQuad(const glm::vec3& position) {
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

	Object3D* Object3D::CreateFromOBJFile(const glm::vec3& position, const char* path) {
		std::vector<float> vertexData = std::vector<float>();
		std::vector<unsigned int> vertexAttributes = std::vector<unsigned int>();

		if(FileLoader::LoadOBJ(path, vertexData, vertexAttributes))
			return new Object3D(position, vertexData, vertexAttributes);
		else
			return nullptr;
	}


#pragma endregion
}