#pragma once
#include "RenderPass.h"
#include "glm/glm.hpp"

namespace BE {
	class Material;
	class ShadowPass : public IRenderPass
	{
	public:

		ShadowPass();
		~ShadowPass();

		virtual void OnRender(std::vector<MeshRenderer*> meshes) override;
	private:

		Material* material;

		glm::mat4 projection;
		glm::mat4 view;
	};
}

