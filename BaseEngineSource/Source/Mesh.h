#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace BE {
	class Mesh {
	public:

		Mesh();
		Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());
		~Mesh();

		void Create();
		void UpdateData(const std::vector<float>& vertexData, const std::vector<unsigned int>& attributes, const std::vector<unsigned int>& indices = std::vector<unsigned int>());

		void Destroy();
		void BindVAO();

		inline bool IsUsingEBO() const { return (ebo > 0) ? true : false; };
		inline unsigned int GetIndicesCount() const { return indicesCount; };

	private:
		
		// TODO: Look into this
		Mesh(Mesh const& mesh) = delete;

		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;

		unsigned int indicesCount;
	};
}

