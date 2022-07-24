#pragma once
#include "IComponent.h"
#include "Entity.h"
#include "glm/glm.hpp"

namespace BE {
	enum class LightType
	{
		DIRECTION,
		SPOT,
		POINT
	};

	class Light : public IComponent
	{
	public:
		Light(const LightType& type);

		virtual void OnStart() {};
		virtual void OnDestroy() {};
		virtual inline const char* GetName() const { return "Light"; }

		inline size_t GetID() const;

		inline void SetColor(const glm::vec3& colour) { colourAndIntensity.r = colour.r; colourAndIntensity.g = colour.g; colourAndIntensity.b = colour.b; }
		inline void SetIntensity(const float& intensity) { colourAndIntensity.a = (intensity < 0) ? 0 : intensity; }
		inline void SetType(const LightType& type) { lightType = type; }
		inline void SetInnerRadius(const float& degrees) { innerRadius = glm::radians(glm::clamp(degrees, 0.0f, 180.0f)); }
		inline void SetOuterRadius(const float& degrees) { outerRadius = glm::radians(glm::clamp(degrees, 0.0f, 180.0f)); }
		inline void SetLength(const float& length) { this->length = glm::max(length, 0.0f); }

		inline glm::vec3 GetColor() const { return glm::vec3(colourAndIntensity.x, colourAndIntensity.y, colourAndIntensity.z); }
		inline glm::vec3 GetDirection() const { return GetEntity()->transform.GetForward(); }
		inline float GetIntensity() const { return colourAndIntensity.w; }
		inline float GetInnerRadius() const { return innerRadius; }
		inline float GetOuterRadius() const { return outerRadius; }
		inline float GetLength() const { return length; }
		inline LightType GetType() const { return lightType; }

		virtual void OnComponentDraw() override;

	protected:

		glm::vec4 colourAndIntensity;
		float length;
		float innerRadius;
		float outerRadius;

	private:
		LightType lightType;

		virtual void OnProcess() override {};
	};

	size_t Light::GetID() const {
		return IComponent::GetComponentID<Light>();
	}
}

