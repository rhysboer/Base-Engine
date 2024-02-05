#pragma once
#ifdef BE_DEBUG
#include "BaseEngine.h"
#include <vector>
#include "glm/glm.hpp"

namespace BE
{
	class Entity;

	class Oracle
	{
	friend class BaseEngine;

	struct Tab
	{
		const char* name;
		void(*func)();
		Tab(const char* name, void(*func)()) : name(name), func(func) { }
	};

	public:


	private:
		static void Init();

		static void Draw();

		static void SceneUI();
		static void SceneUI_AddToSceneHierarchy(Entity* entity);

		static void RenderingUI();
		static void SettingsUI();

		static Entity* selectedEntity;
		static glm::vec3 selectedEntityEuler;
		static std::vector<Tab> tabs;


		struct OracleSettings
		{
			bool drawTransform = true;
			bool drawBones = false;
			bool drawBoundingBox = true;
		};

		static OracleSettings settings;
	};
}
#endif
