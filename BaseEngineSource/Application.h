#pragma once
#include "Engine.h"
#include "FlyCamera.h"
#include "Object3D.h"

#include "glm/matrix.hpp"

class Application : public BaseEngine::Engine {
public:


private:
	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

	BaseEngine::FlyCamera* camera;

	BaseEngine::Object3D* a;
	BaseEngine::Object3D* b;
};