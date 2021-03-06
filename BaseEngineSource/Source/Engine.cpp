#include "Engine.h"
#include <string>

namespace BE {
	GLFWwindow* Engine::window = nullptr;

	void _CallbackFrameBufferResize(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	Engine::Engine() {
		this->isInitialized = false;
		this->window = nullptr;
		this->colourBackground = glm::vec3(0.0f);
	}

	bool Engine::CreateEngine(const char* title, const int& width, const int& height) {
		if(!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->window = glfwCreateWindow(width, height, title, NULL, NULL);

		if(!this->window) {
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(this->window);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return false;
		}

		glViewport(0, 0, width, height);
		InitCallbacks();
		this->isInitialized = true;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();

		return true;
	}

	void Engine::Run() {
		if(!isInitialized)
			return;

		glEnable(GL_DEPTH_TEST); // Depth Testing
		glEnable(GL_BLEND); // Transperent Blend
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	#ifdef DEBUG_BUILD
		Gizmos::Init();
	#endif // DEBUG_BUILD

		// Call On Start
		OnStart();

		while(!glfwWindowShouldClose(this->window)) {
			Time::Update();

			// IMGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			OnUpdate();

			Input::Update();

			glClearColor(this->colourBackground.r, this->colourBackground.g, this->colourBackground.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnRender();

		#ifdef DEBUG_BUILD
			Gizmos::Render();
		#endif // DEBUG_BUILD

			// IMGUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}

		OnEnd();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	glm::vec2 Engine::GetWindowSize() {
		int x, y;
		if(!window) return glm::vec2(0, 0);

		glfwGetWindowSize(window, &x, &y);
		return glm::vec2(x, y);
	}

	GLFWwindow& Engine::GetWindow() {
		return *window;
	}

	void Engine::InitCallbacks() {
		Input::SetCallbacks(this->window);
		glfwSetFramebufferSizeCallback(this->window, _CallbackFrameBufferResize);
	}
}