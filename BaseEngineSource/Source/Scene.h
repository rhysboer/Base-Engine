#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// TODO
// Change entities vector to a map, allowing for much quicker find and deletion


namespace BE {
	class Entity;
	class Camera;
	class BaseEngine;
	class EntityManager;

	/// <summary>
	/// Scene:
	/// TODO
	/// </summary>
	class Scene {
		friend BaseEngine;
	public:

		~Scene() {};
	
		inline EntityManager& GetEntityManager() { return *entityManager; }

	public:

		static Scene* CreateScene(const char* name);
		static Scene* GetScene(const char* name);

	protected:

		EntityManager* entityManager = nullptr;
		std::string name;
		bool isEnabled;

		void OnUpdate();
		void OnRender();

	private:

		Scene(const char* name);

		static void UpdateScenes();
		static void DrawScenes();
		
		static std::unordered_map<std::string, Scene*> scenes;
	};
}
