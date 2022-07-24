#pragma once
#include "IComponent.h"
#include "glm/glm.hpp"

namespace BE {
	class DebugMovement : public IComponent {
	public:

	private:
		
		glm::vec3 euler = glm::vec3(0);

		// Inherited via IComponent
		virtual void OnStart() override;
		virtual void OnProcess() override;
		virtual void OnDestroy() override;
		virtual size_t GetID() const override;
		virtual inline const char* GetName() const { return "Debug Movement"; }
	};
}

