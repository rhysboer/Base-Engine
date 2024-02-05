#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "LightManager.h"
#include "Framebuffer.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "BuiltinShaders.h"
#include "Gizmos.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Entity.h"
#include "UniformBuffer.h"
#include "BaseEngine.h"
#include "TimeManager.h"
#include "GL.h"

#include <algorithm>

#include "Logging.h"


#include "BasePass.h"
#include "ShadowPass.h"

namespace BE {
	std::unordered_map<std::string, BE::Renderer::GlobalTexture> Renderer::globalTextures = std::unordered_map<std::string, Renderer::GlobalTexture>();
	std::vector<size_t> Renderer::defaultLayers = std::vector<size_t>();
	Mesh* Renderer::screenQuad = nullptr;
	Material* Renderer::screenQuadMat = nullptr;
	float Renderer::clearColour[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	bool Renderer::isDirty = true;

	//std::unordered_map<const char*, int> Renderer::passTags = std::unordered_map<const char*, int>();

	Renderer::Renderer(const RenderDesc& desc)
	{
		mainFrameBuffer = desc.mainFrameBuffer; //new Framebuffer(FramebufferType::COLOUR_TEX_DEPTH_TEX);
		passes = desc.renderPasses;
		// TODO: Global Textures

		// Create Screen Quad
		if (!screenQuad) {
			MeshData data = MeshData();
			data.position = {
				-1.0f,  1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f,
				 1.0f, -1.0f, 0.0f,

				-1.0f,  1.0f, 0.0f,
				 1.0f, -1.0f, 0.0f,
				 1.0f,  1.0f, 0.0f
			};
			data.uvs = {
				 0.0f, 1.0f,
				 0.0f, 0.0f,
				 1.0f, 0.0f,

				 0.0f, 1.0f,
				 1.0f, 0.0f,
				 1.0f, 1.0f
			};

			screenQuad = new Mesh(data);
			screenQuad->Apply();
		}

		if (!screenQuadMat) {
			screenQuadMat = new Material(BE_SHADER_SCREENQUAD);
			screenQuadMat->SetUniform("mainTexture", mainFrameBuffer->GetTexture(0));
		}

		glEnable(GL_CULL_FACE);
	}

	Renderer::~Renderer()
	{
		delete screenQuad;
		delete screenQuadMat;

		for (int i = 0; i < passes.size(); i++) {
			delete passes[i];
		}
	}

	void Renderer::Render()
	{
		if (passes.empty()) {
			BE_ERROR("A renderer needs to be created first before rendering");
			return;
		}

		Scene* scene = Scene::GetActiveScene();
		EntityManager* const entities = &scene->GetEntityManager();
		Camera* camera = Camera::GetMainCamera(); // TODO: Change to active camera
		camera->GetTransform().ForceUpdate();

		// Set common shader data
		{
			int x, y;
			BaseEngine::GetWindowSize(x, y);

			UniformBuffer::GetUniformBuffer("Common")->SetValue(0, glm::vec4(x, y, 0, 0));
			UniformBuffer::GetUniformBuffer("Common")->SetValue(1, glm::vec4(BE::Time::DeltaTime(), BE::Time::TotalTime(), 0, 0));
		}

		scene->GetLightManager().BindBuffer();
		camera->BindBuffer();
		
		mainFrameBuffer->Render_Begin();
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// 1. If object changes are dirty, sort out objects into new arrays for each pass depending on renderlayer
		// 2. Attach render targets
		// 3. Render Pass
		// 4. Goto step 2 if more passes
		

		int totalPasses = passes.size();

		if (isDirty) {
			passMeshes.clear();
			passMeshes = std::vector<std::vector<MeshRenderer*>>(totalPasses);
		
			// Grab all scene objects
			auto sceneObjects = entities->GetComponents<MeshRenderer>();
		
			if (sceneObjects != nullptr) {
				MeshRenderer* obj = nullptr;
				for (int i = 0; i < totalPasses; i++) {
					for (int j = 0; j < sceneObjects->Size(); j++) {
						obj = (MeshRenderer*)(*sceneObjects)[j];
		
						if (PassTags::HasTag(obj->GetPassTags(), passes[i]->GetPassTags())) {
							passMeshes[i].push_back(obj);
						}
					}
		
					if (passes[i]->PrioritySort()) {
						std::sort(passMeshes[i].begin(), passMeshes[i].end(), [&](MeshRenderer* a, MeshRenderer* b) {
							return a->GetRenderPriority() > b->GetRenderPriority();
						});
					}
				}
			}
		
			isDirty = false;
		}


		//PassInfo passInfo = PassInfo();
		//passInfo.camera = camera;

		// Render passes
		for (int i = 0; i < totalPasses; i++) {
			passes[i]->OnRender(passMeshes[i]);
		}

		// Render Gizmos
		Gizmos::Render(camera->GetProjectionView());
		
		
		mainFrameBuffer->Render_End();
		mainFrameBuffer->BindTextureColour();
		
		screenQuadMat->SetUniform("mainTexture", mainFrameBuffer->GetTexture(0));
		screenQuadMat->Use();
		

		screenQuad->BindVAO();

		GL::DrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer::RenderQuad()
	{
		if (!screenQuad)
			return;

		screenQuad->BindVAO();
		GL::DrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer::ClearBuffer(const bool& colour, const bool& depth, const bool& stencil)
	{
		glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]);

		unsigned int mask = (colour ? GL_COLOR_BUFFER_BIT : GL_ZERO) | (depth ? GL_DEPTH_BUFFER_BIT : GL_ZERO) | (stencil ? GL_STENCIL_BUFFER_BIT : GL_ZERO);
		glClear(mask);
	}

	ITexture* Renderer::GetGlobalTexture(const char* name)
	{
		auto iter = globalTextures.find(name);
		return iter != globalTextures.end() ? (*iter).second.texture : nullptr;
	}

	void Renderer::SetGlobalTexture(const char* name, ITexture* texture)
	{
		auto iter = globalTextures.find(name);
		if (iter == globalTextures.end()) {
			BE_ERROR("Failed to find global texture with name %s", name);
			return;
		}
		
		(*iter).second.texture = texture;

		const unsigned int size = (*iter).second.uniforms.size();
		for (int i = 0; i < size; i++) {
			(*iter).second.uniforms[i]->SetValue(texture);
		}
	}

	void Renderer::RegisterUniformGlobalTexture(UniformTexture& uniform)
	{
		auto iter = globalTextures.find(uniform.GetName());

		if (iter != globalTextures.end()) {
			(*iter).second.uniforms.push_back(&uniform);
		}
	}

	void Renderer::RegisterGlobalTextures(const std::unordered_map<std::string, ITexture*>& gTextures)
	{
		auto iter = gTextures.begin();
		while (iter != gTextures.end()) {
			GlobalTexture g;
			g.texture = (*iter).second;
			g.uniforms = std::vector<UniformTexture*>();

			globalTextures.emplace((*iter).first, g);
			++iter;
		}
	}

	//void Renderer::AddRenderPass(RenderPass* renderpass)
	//{
	//	//passes.push_back(renderpass);
	//}

	//void ColourRender(const PassInfo& pass, std::vector<IRender*>& renderables)
	//{
	//	for (int i = 0; i < renderables.size(); i++) {
	//		if (renderables[i]->GetMaterial(0)->IsTransparent())
	//			continue;
	//
	//		renderables[i]->OnRender();
	//	}
	//}

	//void TransparentRender(const PassInfo& pass, std::vector<IRender*>& renderables)
	//{
	//	std::sort(renderables.begin(), renderables.end(), [&](IRender* a, IRender* b) { 
	//		return glm::distance2(a->GetEntity()->transform.GetPosition(), pass.camera->GetTransform().GetPosition()) >
	//			glm::distance2(b->GetEntity()->transform.GetPosition(), pass.camera->GetTransform().GetPosition());
	//	});
	//
	//	for (int i = 0; i < renderables.size(); i++) {
	//		if (!renderables[i]->GetMaterial(0)->IsTransparent())
	//			continue;
	//
	//		renderables[i]->OnRender();
	//	}
	//}

	RenderDesc RenderDesc::Default()
	{
		RenderDesc desc;
		desc.mainFrameBuffer = new Framebuffer(FramebufferType::COLOUR_TEX_DEPTH_TEX);
		desc.globalTextures = std::unordered_map<std::string, ITexture*>();
		desc.renderPasses = {
				new ShadowPass(), // Shadow
				new BasePass("Base", PassTags::GetDefaultTag(), desc.mainFrameBuffer) // Colour
		};
		desc.globalTextures = {
			{ BE_GLOBALTEX_SHADOWDEPTH, nullptr }
		};

		return desc;
	}

}

