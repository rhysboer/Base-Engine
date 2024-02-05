#pragma once
#include <unordered_map>
#include <vector>
#include <list>
#include "RenderPass.h"

namespace BE {
	class Material;
	class Mesh;
	class Framebuffer;
	class MeshRenderer;
	class ITexture;
	class UniformTexture;


	struct RenderDesc {
		Framebuffer* mainFrameBuffer = nullptr;
		std::vector<IRenderPass*> renderPasses;
		std::unordered_map<std::string, ITexture*> globalTextures;

		static RenderDesc Default();
	};


	class Renderer 
	{
		friend class BaseEngine;
	public:
		Renderer(const RenderDesc& desc);
		~Renderer();

		const Framebuffer* const GetFrameBuffer() const { return mainFrameBuffer; };

		//static inline void SetDefaultRenderLayers(const std::vector<size_t>& layers) { defaultLayers = std::vector<size_t>(layers); }
		//static inline const std::vector<size_t>& GetDefaultRenderLayers() { return defaultLayers; }
		static inline void SetClearColour(const float& r, const float& g, const float& b, const float& a);

		// TODO: Fix
		inline bool IsValid() const { return true; }

		static void RenderQuad();
		static void ClearBuffer(const bool& colour = false, const bool& depth = false, const bool& stencil = false);
		static void SetDirty() { isDirty = true; }

		inline static Material* const ScreenQuadMaterial() { return screenQuadMat; }

		static ITexture* GetGlobalTexture(const char* name);
		static void SetGlobalTexture(const char* name, ITexture* texture);
		static void RegisterUniformGlobalTexture(UniformTexture& uniform);

	private:
		static void RegisterGlobalTextures(const std::unordered_map<std::string, ITexture*>& gTextures);

		struct GlobalTexture {
			ITexture* texture;
			std::vector<UniformTexture*> uniforms;
		};

		static std::unordered_map<std::string, GlobalTexture> globalTextures;
		static std::vector<size_t> defaultLayers;
		static float clearColour[4];
		static Mesh* screenQuad;
		static Material* screenQuadMat;
		static bool isDirty;
		
		void Render();

		Framebuffer* mainFrameBuffer;
		
		std::vector<IRenderPass*> passes = std::vector<IRenderPass*>();
		std::vector<std::vector<MeshRenderer*>> passMeshes = std::vector<std::vector<MeshRenderer*>>();
	};

	inline void Renderer::SetClearColour(const float& r, const float& g, const float& b, const float& a)
	{
		clearColour[0] = r; 
		clearColour[1] = g; 
		clearColour[2] = b; 
		clearColour[3] = a;
	}
}

