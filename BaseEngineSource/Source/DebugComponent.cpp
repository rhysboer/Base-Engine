#include "DebugComponent.h"
#include "Gizmos.h"
#include "Entity.h"

namespace BE {
	void DebugComponent::OnProcess()
	{
		Gizmos::DrawTransform(GetEntity()->transform.ModelMatrix());
	}

	void DebugComponent::OnStart()
	{
	}

	void DebugComponent::OnDestroy()
	{
	}
}
