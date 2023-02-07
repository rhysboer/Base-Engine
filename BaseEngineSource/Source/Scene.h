#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "EntityManager.h"

// TODO
// Change entities vector to a map, allowing for much quicker find and deletion


namespace BE {
	class Entity;
	class LightManager;
	//class SceneRenderer;

	class Scene {
		friend class BaseEngine;
		friend class RenderSystem;
	public:

		~Scene() {};
	
		inline EntityManager& GetEntityManager() { return *entityManager; }
		inline LightManager& GetLightManager() { return *lightManager; }
		//inline SceneRenderer& GetRenderer() { return *sceneRenderer; }

	public:

		static Scene* CreateScene(const char* name);
		static Scene* GetScene(const char* name);
		static Scene* GetActiveScene() { return activeScene; }
		static unsigned int GetSceneCount() { return scenes.size(); }

	protected:

		EntityManager* entityManager = nullptr;
		LightManager* lightManager = nullptr;
		//SceneRenderer* sceneRenderer = nullptr;

		std::string name;
		bool isEnabled;

	private:

		Scene(const char* name);

		static void RenderActiveScene();
		static void UpdateActiveScene();
		
		static Scene* activeScene;
		static std::unordered_map<std::string, Scene*> scenes;
	};
}

