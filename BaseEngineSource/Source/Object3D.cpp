#include "Object3D.h"

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

void Object3D::Render(BaseCamera& camera) {
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

	if(!mesh->UsingEBO())
		glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndicesCount());
	else
		glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
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

Object3D* CreateObject3D::Plane(const glm::vec3& position, const unsigned int& size) {
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

Object3D* CreateObject3D::Cube(const glm::vec3& position) {
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

Object3D* CreateObject3D::Quad(const glm::vec3& position) {
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

Object3D* CreateObject3D::FromOBJFile(const glm::vec3& position, const char* path) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;

	std::string error;
	std::string warning;

	if(!tinyobj::LoadObj(&attrib, &shapes, &material, &warning, &error, path)) {
		printf("ERROR: Object3D - Failed to load object at (%s)\n", path);
		return nullptr;
	}

	if(!error.empty()) {
		printf("ERROR: Object3D - %s\n", error.c_str());
		return nullptr;
	}

	if(!warning.empty())
		printf("WARNING: Object3D - %s\n", warning.c_str());

	std::vector<float> vertex_data = std::vector<float>();
	std::vector<unsigned int> vertex_attributes = std::vector<unsigned int>({ 3 });

	if(!attrib.normals.empty())
		vertex_attributes.push_back(3);
	if(!attrib.colors.empty())
		vertex_attributes.push_back(3);
	if(!attrib.texcoords.empty())
		vertex_attributes.push_back(2);

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
				vertex_data.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				vertex_data.push_back(-attrib.vertices[3 * idx.vertex_index + 1]);
				vertex_data.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

				// Vertex Normals
				if(!attrib.normals.empty()) {
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 0]);
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 1]);
					vertex_data.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}

				// Vertex Color
				if(!attrib.colors.empty()) {
					vertex_data.push_back(attrib.colors[3 * idx.normal_index + 0]);
					vertex_data.push_back(attrib.colors[3 * idx.normal_index + 1]);
					vertex_data.push_back(attrib.colors[3 * idx.normal_index + 2]);
				}

				// Vertex Texcoords
				if(!attrib.texcoords.empty()) {
					vertex_data.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
					vertex_data.push_back(1.0 - attrib.texcoords[2 * idx.texcoord_index + 1]);
				}
			}

			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return new Object3D(position, vertex_data, vertex_attributes);
}


#pragma endregion