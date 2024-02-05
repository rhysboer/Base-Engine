#include "LightManager.h"
#include "UniformBuffer.h"
#include "Framebuffer.h"
#include "glm/glm.hpp"
#include "Light.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Renderer.h"

#include "Logging.h"

namespace BE {
	LightManager::LightManager(Scene* scene) 
		: scene(scene), ambientLight(0.1f), countUniformIndex(-1), ambientUniformIndex(-1), shadowBuffer(nullptr)
	{ }

	LightManager::~LightManager()
	{
		if (IsUsingShadowMap()) {
			delete shadowBuffer;
			shadowBuffer = nullptr;
		}
	}

	void LightManager::CreateShadowMap(const int& width, const int& height)
	{
		if (IsUsingShadowMap())
			return;

		TextureDesc desc = {};
		desc.wrapping = Wrapping::BORDER;
		desc.borderColour[0] = 1.0f;
		desc.borderColour[1] = 1.0f;
		desc.borderColour[2] = 1.0f;
		desc.borderColour[3] = 1.0f;

		shadowWidth = width;
		shadowHeight = height;
		shadowBuffer = new Framebuffer(FramebufferType::DEPTH_TEX, shadowWidth, shadowHeight, desc);

		BE::Renderer::SetGlobalTexture(BE_GLOBALTEX_SHADOWDEPTH, (ITexture*)shadowBuffer->GetDepthTexture());
	}

	void LightManager::BindBuffer()
	{
		ComponentArray* const lights = scene->GetEntityManager().GetComponents<Light>();
		
		if (lights == nullptr)
			return;

		UniformBuffer* buffer = UniformBuffer::GetUniformBuffer("Lights");
		
		int size = lights->Size(), index = 0;
		for (int i = 0; i < size; i++) {
			auto light = (Light*)lights->AtIndex(i);

			//auto var = light->GetDirection();
			//BE_LOG("%f, %f, %f", var.x, var.y, var.z);

			buffer->SetValue(index + 0, glm::vec4(light->GetColor(), light->GetIntensity()));
			buffer->SetValue(index + 1, glm::vec4(light->GetDirection(), light->GetType()));
			buffer->SetValue(index + 2, glm::vec4(light->GetEntity()->transform.GetPosition(), 0));
			buffer->SetValue(index + 3, glm::vec4(light->GetLength(), light->GetInnerRadius(), light->GetOuterRadius(), 0));

			index += 4;
		}

		if (countUniformIndex == -1)
			countUniformIndex = buffer->FindIndex("Lights.count");
		if (ambientUniformIndex == -1)
			ambientUniformIndex = buffer->FindIndex("Lights.ambient");

		buffer->SetValue(countUniformIndex, size);
		buffer->SetValue(ambientUniformIndex, ambientLight);
	}
}