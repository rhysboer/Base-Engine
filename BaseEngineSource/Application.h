#pragma once
#include "BaseEngine.h"
#include "Scene.h"
#include "Texture.h"

namespace BE {
	class Debugger;
	class MeshData;
	class Material;
}

class Application : public BE::BaseEngine {
public:

	Application(const BE::EngineDesc& engineDesc);
	~Application();

private:

	virtual void OnFrameUpdate();

	BE::Texture* terrainTexture;
	BE::Scene* mainScene;

	BE::Material* wormMaterial;
};

