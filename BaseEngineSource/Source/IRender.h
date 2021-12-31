#pragma once
#include "IComponent.h"

namespace BE {
	class Mesh;
	class Material;
	class Camera;
	class IRender : public IComponent {
	public:

		IRender();
		~IRender();

		virtual void OnStart() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnRender(Camera* const camera) = 0;
		inline size_t GetID() const;

	protected:

		Mesh* mesh;
		Material* material;

	private:

		virtual void OnProcess() override {};
	};

	// Makes every renderer use the same rendering ID
	size_t IRender::GetID() const {
		return IComponent::GetComponentID<IRender>();
	}
}

