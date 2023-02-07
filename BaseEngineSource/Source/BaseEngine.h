#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace BE {
	enum GraphicsAPI
	{
		OpenGL,
		Vulkan,
		DirectX11,
		DirectX12
	};

	struct EngineDesc
	{
		const char* name = "Base Engine Project";
		int width = 1280;
		int height = 720;
		GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;

		// OpenGL
		int8_t openGl_versionMinor = 3;
		int8_t openGL_versionMajor = 3;
	};

	class Renderer;
	class ShaderManager;
	class BaseEngine {
		friend ShaderManager;
	public:
		~BaseEngine();

		// Creating the Engine
		static BaseEngine* CreateEngine(const EngineDesc& desc);

		void Run();
		inline void Stop() { isStopping = true; }

		// Utility Functions
		static inline BaseEngine* const GetEngine() { return engine; }
		static inline void GetWindowSize(int& x, int& y) { glfwGetWindowSize(engine->window, &x, &y); }
		static inline GLFWwindow* const GetWindow();
		static inline Renderer* const GetRenderer();

	private:
		static BaseEngine* engine;

		BaseEngine();

		bool isStopping = false;


		GLFWwindow* window;
		Renderer* renderer;
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

