#pragma once
#include "RenderPass.h"

namespace BE {
	class BasePass : public IRenderPass
	{
	public:

		BasePass(const char* debugName, const PassTag& tags, Framebuffer* buffer);
		~BasePass() {}

	private:

		Framebuffer* buffer;
		virtual void OnRender(std::vector<MeshRenderer*> meshes) override;
	};
}


