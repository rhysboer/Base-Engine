#pragma once
#include "IRender.h"

namespace BE {
	class MeshRenderer : public IRender {
	public:

		MeshRenderer(Mesh* mesh = nullptr, Material* material = nullptr);
		~MeshRenderer() {};

	protected:
		// Inherited via IRender
		virtual void OnStart() override;
		virtual void OnDestroy() override;
		virtual void OnRender(Camera* const camera) override;

		// https://gamedev.stackexchange.com/questions/133615/how-do-you-store-uniform-data
	};
}
