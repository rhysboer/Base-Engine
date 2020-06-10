#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Mesh {
public:

	Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
	~Mesh();

	void Destroy();
	void BindVAO();

	bool UsingEBO() const;
	unsigned int GetIndicesCount() const;

private:

	Mesh(Mesh const & mesh) = delete;
	 
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int indicesCount;
};

