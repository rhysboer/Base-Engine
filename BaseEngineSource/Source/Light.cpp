#include "Light.h"

#include "imgui.h"

namespace BE {
	Light::Light(const LightType& type)
		: lightType(type), colourAndIntensity(1, 1, 1, 1), length(10), innerRadius(glm::radians(40.0f)), outerRadius(glm::radians(50.0f))
	{
	}

	void Light::OnComponentDraw()
	{
		int type = (int)lightType;

		if (ImGui::Combo("##Type", &type, "Directional\0Spot\0Point")) {
			lightType = (LightType)type;
		}

		switch (lightType)
		{
		case BE::LightType::DIRECTION: {

			break;
		}
		case BE::LightType::SPOT:
		{
			float inner = glm::degrees(innerRadius);
			float outer = glm::degrees(outerRadius);

			ImGui::Text("Inner Radius");
			ImGui::DragFloat("##Inner", &inner, 0.2f, 0.0f, outer);
			ImGui::Text("Outer Radius");
			ImGui::DragFloat("##Outer", &outer, 0.2f, inner, 180.0f);

			innerRadius = glm::radians(inner);
			outerRadius = glm::radians(outer);

			break;
		}
		case BE::LightType::POINT:
			break;
		}
	}
}