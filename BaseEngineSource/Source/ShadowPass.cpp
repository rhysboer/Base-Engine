#include "ShadowPass.h"
#include "MeshRenderer.h"
#include "LightManager.h"
#include "Scene.h"
#include "Material.h"
#include "ShaderManager.h"
#include "GL.h"
#include "Entity.h"
#include "UniformBuffer.h"

#include "Light.h"

namespace BE {
	ShadowPass::ShadowPass() : IRenderPass("ShadowPass", PassTags::GetDefaultTag()), material(nullptr)
	{

	}

	ShadowPass::~ShadowPass()
	{
		delete material;
	}

	void ShadowPass::OnRender(std::vector<MeshRenderer*> meshes)
	{
		if (material == nullptr)
			material = new Material(BE_SHADER_SHADOW);

		ComponentArray* const lights = Scene::GetActiveScene()->GetEntityManager().GetComponents<Light>();
		auto light = lights->AtIndex(0)->GetTransform();

		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
		view = glm::lookAt(light.GetPosition(), -light.GetForward() + light.GetPosition(), light.GetUp());

		Camera::BindBuffer(projection, view);

		auto& lightManager = Scene::GetActiveScene()->GetLightManager();
		lightManager.GetShadowMapFrameBuffer()->Render_Begin();

		GL::SetFaceCulling(GL::Face::FRONT_FACE);

		Mesh* mesh = nullptr;
		for (int i = 0; i < meshes.size(); i++) {
			if (meshes[i]->GetCastShadows()) {
				if (!meshes[i]->CanRender())
					continue;

				material->SetUniform("model", meshes[i]->GetEntity()->transform.ModelMatrix());
				material->Use();

				mesh = meshes[i]->GetMesh();
				for (int j = 0; j < mesh->GetMeshCount(); j++) {
					GL::DrawMesh(*mesh, j);
				}
			}
		}

		GL::SetFaceCulling(GL::Face::BACK_FACE);

		lightManager.GetShadowMapFrameBuffer()->Render_End(true);
	
		UniformBuffer::GetUniformBuffer("ShadowMap")->SetValue(0, projection * view);
	
		// Rebind the main camera
		Camera::GetMainCamera()->BindBuffer();

		/*
		
			When setting the passtags on an object, have the ability to attach new shaders to use for that pass

			Write a new piece of software that breaks shader source files into new seperate files and adds "variations" on these shaders.
			So we can write a new shader, and add to the header that we want to auto generate a "shadow" variation. Which will overwrite the fragment shader with the shadow source code.

			In our code we will need to adjust how we can use these "variation" shaders when needed. 
			When binding a shader we can maybe set an arguement to force it to use the variation or "sub pass"

			Decide whether a shader variation/subpass should be stored within the material or shader.
		



			- If marked as shader override, In a renderpass, check if shader as our variation, else use a default one.
			- 
		*/
	}
}