#pragma once
#include "Engine.h"
#include "Camera3D.h"
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

	Camera3D* camera;
	Object3D* object;
};