#pragma once
#include "IComponent.h"
#include <vector>
#include "Mesh.h"

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
		virtual void OnRender(const unsigned int& subMeshIndex) = 0;
		inline size_t GetID() const;


		inline unsigned int GetMeshCount() const { return (mesh != nullptr) ? mesh->GetMeshCount() : 0; }

		inline Material* const GetMaterial(const unsigned int& materialIndex = 0) { return (materials.size() > 0) ? materials[glm::min((int)materialIndex, (int)materials.size() - 1)] : nullptr; }
		void SetMaterials(const std::vector<Material*> materials);

		virtual void OnComponentDraw() override;

	protected:

		virtual void OnProcess() override {};
		
		Mesh* mesh;
		std::vector<Material*> materials;
	private:

	};

	// Forces every renderer to use the same component ID
	size_t IRender::GetID() const {
		return IComponent::GetComponentID<IRender>();
	}
}

