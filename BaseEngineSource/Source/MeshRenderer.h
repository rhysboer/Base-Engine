#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderPass.h"

#define BE_RENDERPRI_HIGHEST 32767
#define BE_RENDERPRI_LOWEST -32768

namespace BE {
	class MeshRenderer : public IComponent {
	public:
		MeshRenderer();
		MeshRenderer(Mesh* mesh);
		MeshRenderer(Mesh* mesh, Material* material);
		MeshRenderer(const MeshData& mesh);
		MeshRenderer(const MeshData& mesh, Material* material);
		MeshRenderer(Mesh* mesh, std::vector<Material*> materials);
		MeshRenderer(Mesh* mesh, Material** const materials, const unsigned int& size);
		MeshRenderer(const MeshData& mesh, std::vector<Material*> materials);
		MeshRenderer(Material* material);
		MeshRenderer(Material** materials, const unsigned int& size);
		MeshRenderer(std::vector<Material*> materials);

		~MeshRenderer() {};

		//void OnRender();

		void SetMaterial(Material& material, const unsigned int& index = 0);
		void SetMaterials(const std::vector<Material*> materials);
		void SetMesh(BE::Mesh* mesh);
		void SetMesh(BE::MeshData& mesh);
		inline void SetRenderPriority(const short& priority);
		inline void SetShadowCasting(const bool& cast);

		void BindMaterial(const unsigned int& subMesh = 0) const;

		size_t GetID() const override { return IComponent::GetComponentID<MeshRenderer>(); }
		inline const char* GetName() const override { return "Mesh Renderer"; }

		inline Material* const GetMaterial(const unsigned int& materialIndex = 0) const { return (materials.size() > 0) ? materials[glm::min((int)materialIndex, (int)materials.size() - 1)] : nullptr; }
		inline Mesh* const GetMesh() const { return mesh != nullptr ? mesh : nullptr; }
		inline PassTag GetPassTags() const { return passTags; }
		inline short GetRenderPriority() const { return renderPriority; }
		inline bool GetCastShadows() const { return castShadows; }
		inline bool CanRender() const { return render; }

	protected:
		// Inherited via IRender
		virtual void OnStart() override {}
		virtual void OnDestroy() override {}

		// Inherited via IComponent
		virtual void OnProcess() override {}

#ifdef BE_DEBUG
		virtual void OnComponentDraw() override;
#endif

		// https://gamedev.stackexchange.com/questions/133615/how-do-you-store-uniform-data

	private:

		PassTag passTags = 0;
		std::vector<Material*> materials;
		Mesh* mesh;

		short renderPriority = 0;
		bool castShadows = true;
		bool render = true;
	};

	inline void MeshRenderer::SetRenderPriority(const short& priority)
	{
		renderPriority = priority;
	}

	inline void MeshRenderer::SetShadowCasting(const bool& cast)
	{
		castShadows = cast;
	}
}
