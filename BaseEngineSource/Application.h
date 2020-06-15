#pragma once
#include "Engine.h"
#include "Camera.h"
#include "Object3D.h"

#include "glm/matrix.hpp"

class Application : public Engine {
public:


private:
	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

	Camera* camera;

	Object3D* a;
	Object3D* b;
};