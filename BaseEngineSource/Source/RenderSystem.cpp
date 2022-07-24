#include "RenderSystem.h"
#include "Camera.h"
#include "Scene.h"
#include "UniformBuffer.h"

#include "EntityManager.h"
#include "LightManager.h"

#include <algorithm>

#include "IRender.h"
#include "Material.h"
#include "Transform.h"
#include "Gizmos.h"

namespace BE {
	void RenderSystem::Render()
	{
		auto iter = Scene::GetBegin();
		auto end = Scene::GetEnd();

		Camera* camera = nullptr;

		while (iter != end) {
			Scene* scene = iter->second;

			scene->GetLightManager().BindLights();

			camera = (Camera*)scene->GetEntityManager().GetComponents<BE::Camera>()->AtIndex(0);


			UniformBuffer::GetUniformBuffer("Camera")->SetValue(0, camera->GetProjectionView());


			auto renders = scene->GetEntityManager().GetComponents<BE::IRender>();
			int size = renders != nullptr ? renders->Size() : 0;

			std::vector<glm::ivec2> transparentMeshes = std::vector<glm::ivec2>();

			// Opaque Pass
			for (int i = 0; i < size; i++) {
				IRender* renderer = (IRender*)renders->AtIndex(i);

				if (!renderer->IsActive())
					continue;

				unsigned int meshCount = renderer->GetMeshCount();
				for (int j = 0; j < meshCount; j++) {
					Material* material = renderer->GetMaterial();
					if (material != nullptr && material->IsTransparent()) {
						transparentMeshes.push_back(glm::ivec2(i, j));
						continue;
					}

					renderer->OnRender(j);
				}
			}

			// Transparent Pass
			std::sort(transparentMeshes.begin(), transparentMeshes.end(), [&camera, &renders](const auto& a, const auto& b) {
				return glm::distance(camera->GetTransform().GetPosition(), ((IRender*)renders->AtIndex(a.x))->GetTransform().GetPosition()) > 
					glm::distance(camera->GetTransform().GetPosition(), ((IRender*)renders->AtIndex(b.x))->GetTransform().GetPosition());
			});

			for (int i = 0; i < transparentMeshes.size(); i++) {
				IRender* renderer = (IRender*)renders->AtIndex(transparentMeshes[i].x);

				if (!renderer->IsActive())
					continue;

				renderer->OnRender(transparentMeshes[i].y);
			}

			Gizmos::Render(camera->GetProjectionView());
			iter++;
		}
	}
}