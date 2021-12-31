#pragma once
#include "IComponent.h"

class ExampleComponent : public BE::IComponent {
public:
	// Inherited via IComponent
	virtual void OnProcess() override;

	// Inherited via IComponent
	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual size_t GetID() const override;
};

