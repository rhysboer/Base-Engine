#pragma once
#include "BaseEngine.h"

#include "Scene.h"

namespace BE {
	class Debugger;
}
class Application : public BE::BaseEngine  {
public:

private:

	BE::Scene* mainScene;


	//BE::Ray ray = BE::Ray(glm::vec3(0), glm::vec3(0));

	// Inherited via BaseEngine
	virtual void OnEngineInit() override;
	virtual void OnEngineDestroy() override;

	virtual void OnEngineUpdate() override;
	virtual void OnEngineRender() override;
};

