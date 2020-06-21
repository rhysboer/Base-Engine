#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Input.h"
#include "Time.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#include "Gizmos.h"
#endif // DEBUG_BUILD

class Engine {
public:
	// Creates the engine & window
	bool CreateEngine(const char* title, const int& width, const int& height);

	// Begins the engine
	void Run();

	// Static Variables
	static glm::vec2 GetWindowSize();
	static GLFWwindow& GetWindow();

protected:
	// Constructors
	Engine();

	// Virtual Functions
	virtual void OnStart() = 0;
	virtual void OnEnd() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	// Variables
    static GLFWwindow* window;

	glm::vec3 colourBackground;
private:
	
	void InitCallbacks();

	bool isInitialized;
};

