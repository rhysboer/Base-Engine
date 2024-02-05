#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "EngineDesc.h"

namespace BE {
	class RenderDesc;
	class Renderer;
	class PhysicsManager;
	class ShaderManager;
	class BaseEngine {
		friend ShaderManager;
	public:
		~BaseEngine();


		void Run();
		inline void Stop() { isStopping = true; }

		// Utility Functions
		static inline BaseEngine* const GetEngine() { return engine; }
		static inline void GetWindowSize(int& x, int& y) { glfwGetWindowSize(engine->window, &x, &y); }
		static inline GLFWwindow* const GetWindow();
		static inline Renderer* const GetRenderer();

		static inline const EngineDesc& const GetEngineDesc() { return engine->desc; }

	protected:
		BaseEngine(const EngineDesc& desc);

		// Creating the Engine
		void CreateEngine(const RenderDesc& renderDesc);

		virtual void OnFrameUpdate() { }

	private:
		static BaseEngine* engine;

		bool isStopping = false;

		GLFWwindow* window;
		Renderer* renderer;
		PhysicsManager* physicsManager;
		EngineDesc desc;
	};

	inline GLFWwindow* const BaseEngine::GetWindow()
	{
		return (engine != nullptr) ? engine->window : nullptr;
	}

	inline Renderer* const BaseEngine::GetRenderer() 
	{
		return (engine != nullptr) ? engine->renderer : nullptr;
	}
}

