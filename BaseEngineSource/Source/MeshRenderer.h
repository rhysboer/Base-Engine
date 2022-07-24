#pragma once
#include "IRender.h"

namespace BE {
	class MeshRenderer : public IRender {
	public:
		MeshRenderer();
		MeshRenderer(Mesh* mesh);
		MeshRenderer(Mesh* mesh, Material* material);
		MeshRenderer(Mesh* mesh, std::vector<Material*> materials);
		MeshRenderer(Mesh* mesh, Material** materials, const unsigned int& size);
		MeshRenderer(Material* material);
		MeshRenderer(Material** materials, const unsigned int& size);
		MeshRenderer(std::vector<Material*> materials);

		~MeshRenderer() {};

		virtual inline const char* GetName() const override { return "Mesh Renderer"; }

	protected:
		// Inherited via IRender
		virtual void OnStart() override {}
		virtual void OnDestroy() override {}
		virtual void OnRender(const unsigned int& subMeshIndex) override;

		// https://gamedev.stackexchange.com/questions/133615/how-do-you-store-uniform-data
	};
}
