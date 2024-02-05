#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "Framebuffer.h"

#define BE_PASSTAG_DEFAULT "default"

namespace BE {
	class MeshRenderer;
	class Renderer;
	struct PassInfo;

	typedef unsigned int PassTag;

	class PassTags {
	public:

		static PassTag RegisterTag(const char* name);

		static PassTag GetTag(const char* name);
		static PassTag GetTags(std::vector<const char*> names);
		static PassTag GetDefaultTag();
		static std::vector<std::string> GetAllTags();

		static inline bool HasTag(const PassTag& passA, const PassTag& passB) { return (passA & passB) != 0; }

		// TODO: Redundent?
		static inline bool ShareTag(const PassTag& passA, const PassTag& passB) { return (passA & passB) == 0; }

	private:
	
		static void RegisterDefaultTag();

		PassTags() {};
		~PassTags() {};

		static PassTag defaultTag;
		static std::unordered_map<std::string, PassTag> tags;
	};


	class IRenderPass {
	public:

		// TODO: add render settings (stencil buffer and stuff)
		IRenderPass(const char* debugName, const PassTag& tags) : debugName(debugName), tags(tags) {}

		virtual void OnRender(std::vector<MeshRenderer*> meshes) = 0;
		
		inline PassTag GetPassTags() const { return tags; }


		inline bool PrioritySort() const { return prioritySort; }

	protected:

		bool prioritySort = true;

	private:

		std::string debugName;
		PassTag tags;

	};


	//struct RenderPass {
	//	std::string name;
	//	bool renderTargetEnable = true; // If enabled, and RenderTarget is null, pass will use the default framebuffer
	//	bool depthStencilEnable = true;// If enabled, and RenderTarget is null, pass will use the default framebuffer
	//	BE::Framebuffer* renderTexture = nullptr;
	//	BE::Framebuffer* depthStencilTexture = nullptr;
	//	//std::unordered_set<size_t> renderLayers = std::unordered_set<size_t>();
	//	short shaders[2] = { -1, -1 }; // overides for VERT & FRAG 
	//	bool frustumCull = true;
	//
	//	void* data = nullptr;
	//	void(*renderFunc)(const PassInfo&, std::vector<IRender*>&);// = nullptr;
	//};
	//
	//struct PassInfo {
	//	Camera* camera;
	//	RenderPass* passDesc;
	//};

	//class IRenderPass {
	//public:
	//	~IRenderPass() {}
	//
	//	virtual void Render(PassInfo& pass) = 0;
	//
	//protected:
	//	IRenderPass() {}
	//};
}
