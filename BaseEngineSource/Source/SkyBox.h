#pragma once
#include "Object3D.h"
#include "Cubemap.h"

#include "Camera.h"

namespace BE {
	class SkyBox {
	public:

		SkyBox(const char* cubemapPath, Shader* shader);
		~SkyBox();

		void Render(Camera& camera);
		Shader* GetShader();

	private:

		Cubemap* texture;
		Object3D* cube;
	};
}
