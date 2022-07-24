#pragma once
#include "BaseEngine.h"

class Application : public BE::BaseEngine
{
	public:

	private:

	// Inherited via BaseEngine
	virtual void OnEngineInit() override;
	virtual void OnEngineDestroy() override;

	void OnEngineUpdate() override;
};

