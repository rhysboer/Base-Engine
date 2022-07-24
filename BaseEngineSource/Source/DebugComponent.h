#pragma once
#include "IComponent.h"

namespace BE {
	class DebugComponent : public IComponent
	{
	public:

		// Inherited via IComponent
		virtual void OnStart() override;
		virtual void OnProcess() override;
		virtual void OnDestroy() override;

	private:

		virtual size_t GetID() const override { return IComponent::GetComponentID<DebugComponent>(); }
		virtual const char* GetName() const override { return "Debug Component"; }
	};
}

