#pragma once
#include "IComponent.h"


class Chunk : public BE::IComponent
{
	// Inherited via IComponent
	virtual void OnStart() override;
	virtual void OnProcess() override;
	virtual void OnDestroy() override;
	virtual size_t GetID() const override;
	virtual const char* GetName() const override;
};

