#pragma once
#include <vector>
#include <set>
#include "IComponent.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include "RenderPass.h"

// TODO:
// [ ] - Remove the ability to have multiple materials & meshes on one IRender

namespace BE {
	class Mesh;
	class Material;
	class IRender : public IComponent {
	public:
		IRender();
		~IRender();
		


		virtual void OnStart() = 0;
		virtual void OnDestroy() = 0;
		virtual inline const char* GetName() const = 0;
		virtual void OnRender() = 0;
		inline size_t GetID() const;

		inline unsigned int GetMeshCount() const { return (mesh != nullptr) ? mesh->GetMeshCount() : 0; }
		inline PassTag GetPassTags() const { return passTags; }
		inline Material* const GetMaterial(const unsigned int& materialIndex = 0) { return (materials.size() > 0) ? materials[glm::min((int)materialIndex, (int)materials.size() - 1)] : nullptr; }
		inline Mesh* const GetMesh() const { return mesh != nullptr ? mesh : nullptr; }

		void SetMaterials(const std::vector<Material*> materials);
		void SetMesh(BE::Mesh* mesh);
		void SetMesh(BE::MeshData& mesh);

		// Debug
		virtual void OnComponentDraw() override;

	protected:

		virtual void OnProcess() override {};
		
		std::vector<Material*> materials;
		PassTag passTags = 0;
		Mesh* mesh;
	};

	// Forces every renderer to use the same component ID
	size_t IRender::GetID() const {
		return IComponent::GetComponentID<IRender>();
	}
}

