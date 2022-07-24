#include "BaseEngine.h"
#include "Input.h"
#include "Logging.h"
#include "TimeManager.h"
#include "Gizmos.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "BEGlobal.h"

#include "RenderSystem.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Callback_FrameBufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//BE::CameraManager::SetAllDirty();
}

namespace BE {
	GLFWwindow* BaseEngine::window = nullptr;

	bool BaseEngine::CreateEngine(const char* title, const int& width, const int& height) {
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
		
		RegisterCallbacks();

		this->isInitialized = true;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();

		ShaderManager::InitBaseShaders();
		Gizmos::Init();
		BEGlobal::Init();

		return true;
	}

	void BaseEngine::StartEngine() {
		if(!isInitialized) {
			Logging::Error("BaseEngine: Engine has not been initialized");
			return;
		}

		glEnable(GL_DEPTH_TEST); // Depth Testing
		glEnable(GL_BLEND); // Transparent Blend
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Call On Start
		OnEngineInit();

		while(!glfwWindowShouldClose(this->window)) {
			// --  IMGUI UPDATE
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			// -- UPDATE
			{
				Time::Update();
				
				Scene::UpdateScenes();

				OnEngineUpdate();
			}

			// -- CLEAR SCREEN
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// -- RENDER
			{
				//Scene::DrawScenes();
				RenderSystem::Render();
				OnEngineRender();
			}

			// --  IMGUI RENDER
			{
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}

			// -- SWAP BUFFERS
			glfwSwapBuffers(this->window);
			
			Input::Update();
			glfwPollEvents();
		}

		// -- DESTROY ENGINE
		OnEngineDestroy();

		// -- IMGUI TERMINATE
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void BaseEngine::RegisterCallbacks() {
		Input::SetCallbacks(this->window);

		// -- Register Callbacks Here
		glfwSetFramebufferSizeCallback(this->window, Callback_FrameBufferResize);
	}
}


