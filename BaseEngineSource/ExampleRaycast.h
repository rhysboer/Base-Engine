#pragma once
#include "Camera.h"
#include "IComponent.h"
#include "Raycast.h"

class ExampleRaycast : public BE::IComponent {
public:

	// Inherited via IComponent
	virtual void OnStart() override;
	virtual void OnProcess() override;
	virtual void OnDestroy() override;
	virtual size_t GetID() const override;

private:

	BE::Camera * camera = nullptr;


	BE::Ray* ray = nullptr;
	BE::RayHit rayHit = BE::RayHit();
};

