#pragma once
#include "Engine.h"

class Application : public Engine {
public:


private:

	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

};