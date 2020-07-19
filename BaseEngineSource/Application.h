#pragma once
#include "Engine.h"
#include "FlyCamera.h"
#include "Object3D.h"

#include "glm/matrix.hpp"

class Application : public BE::Engine {
public:


private:
	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

	BE::FlyCamera* camera;

	BE::Object3D* a;
	BE::Object3D* b;
};