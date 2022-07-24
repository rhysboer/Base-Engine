#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// TODO
// Change entities vector to a map, allowing for much quicker find and deletion


namespace BE {
	class Entity;
	class EntityManager;
	class LightManager;

	class Scene {
		friend class BaseEngine;
		friend class RenderSystem;
	public:

		~Scene() {};
	
		inline EntityManager& GetEntityManager() { return *entityManager; }
		inline LightManager& GetLightManager() { return *lightManager; }

	public:

		static Scene* CreateScene(const char* name);
		static Scene* GetScene(const char* name);
		static unsigned int GetSceneCount() { return scenes.size(); }

	protected:

		EntityManager* entityManager = nullptr;
		LightManager* lightManager = nullptr;

		std::string name;
		bool isEnabled;

		void OnUpdate();
		void OnRender();

		static std::unordered_map<std::string, Scene*>::iterator GetBegin() { return scenes.begin(); }
		static std::unordered_map<std::string, Scene*>::iterator GetEnd() { return scenes.end(); }

	private:

		Scene(const char* name);

		static void UpdateScenes();
		static void DrawScenes();
		
		static std::unordered_map<std::string, Scene*> scenes;
	};
}

