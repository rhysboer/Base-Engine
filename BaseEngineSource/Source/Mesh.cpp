#include "Mesh.h"

namespace BaseEngine {
	Mesh::Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices) : ebo(0) {

		// Generate Buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

		if(!indices.empty()) {
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}

		unsigned int attributesPerVertex = 0;
		for(int i = 0; i < attributes.size(); i++)
			attributesPerVertex += attributes[i];

		unsigned int offset = 0;
		for(int i = 0; i < attributes.size(); i++) {
			glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void*)(offset * sizeof(float)));
			glEnableVertexAttribArray(i);

			offset += attributes[i];
		}

		indicesCount = (indices.empty()) ? (sizeof(float) * vertexData.size()) / attributesPerVertex : indices.size();

		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		Destroy();
	}

	void Mesh::Destroy() {
		if(vao != 0) {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);

			if(ebo != 0)
				glDeleteBuffers(1, &ebo);
		}
	}

	void Mesh::BindVAO() {
		glBindVertexArray(vao);
	}

	bool Mesh::UsingEBO() const {
		return (ebo > 0) ? true : false;
	}

	unsigned int Mesh::GetIndicesCount() const {
		return indicesCount;
	}
}