#pragma once
#include "BaseEngine.h"
#include "Scene.h"

namespace BE {
	class Debugger;
	class MeshData;
}

class Application {
public:
	Application();
	~Application();

	void Play();

private:

	void OnUpdate(const void* const);

	BE::BaseEngine* engine;
	BE::Scene* mainScene;
};

