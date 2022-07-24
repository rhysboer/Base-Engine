#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace BE {
	class ShaderManager;
	class BaseEngine {
		friend ShaderManager;
	public:
		
		// Creating the Engine
		bool CreateEngine(const char* title, const int& width, const int& height);
		void StartEngine();

		// Utility Functions
		static inline void GetWindowSize(int& x, int& y) { glfwGetWindowSize(window, &x, &y); }
		static inline GLFWwindow& GetWindow() { return *window; }

	protected:

		// Virtual Functions
		virtual void OnEngineInit() = 0;
		virtual void OnEngineDestroy() = 0;

		virtual void OnEngineUpdate() {};
		virtual void OnEngineRender() {};

		virtual void OnCallbackRegister() {}

		// Variables
		static GLFWwindow* window;
	private:

		void RegisterCallbacks();

		bool isInitialized;
	};
}

