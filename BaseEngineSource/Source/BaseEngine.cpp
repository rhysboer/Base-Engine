#include "BaseEngine.h"
#include "Input.h"
#include "Logging.h"
#include "TimeManager.h"
#include "Gizmos.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "BEGlobal.h"
#include "EventSystem.h"
#include "Renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Callback_WindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	glm::vec2 size = glm::vec2(width, height);
	BE::EventSystem::TriggerEvent(BE_EVENT_WINDOW_RESIZE, &size);
}

namespace BE {
	BaseEngine* BaseEngine::engine = nullptr;

	BaseEngine::BaseEngine() : window(nullptr), renderer(nullptr)
	{ }

	BaseEngine::~BaseEngine()
	{ 
		delete renderer;
	}

	BaseEngine* BaseEngine::CreateEngine(const EngineDesc& desc) {
		if (engine)
			return engine;

		engine = new BaseEngine();

		if (!glfwInit()) {
			delete engine;
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc.openGL_versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc.openGl_versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		engine->window = glfwCreateWindow(desc.width, desc.height, desc.name, NULL, NULL);

		if(!engine->window) {
			glfwTerminate();
			delete engine;
			return nullptr;
		}

		glfwMakeContextCurrent(engine->window);
		//glfwSwapInterval(0);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			delete engine;
			return nullptr;
		}

		glViewport(0, 0, desc.width, desc.height);
		
		// -- Register Callbacks Here
		glfwSetWindowSizeCallback(engine->window, Callback_WindowResize);
		Input::SetCallbacks(engine->window);

		// Initiate
		engine->renderer = new Renderer();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(engine->window, true);
		// TODO: Change with desc.opengl_VersionMajor/Minor
		ImGui_ImplOpenGL3_Init("#version 330"); 
		ImGui::StyleColorsDark();

		EventSystem::CreateEvent(BE_EVENT_WINDOW_RESIZE);
		EventSystem::CreateEvent(BE_EVENT_WINDOW_MINIMISE);
		EventSystem::CreateEvent(BE_EVENT_ENGINE_UPDATE);
		ShaderManager::InitBaseShaders();
		BEGlobal::Init();

		return engine;
	}

	void BaseEngine::Run() {
		if(!engine) {
			Logging::Error("BaseEngine: Error creating engine");
			return;
		}

		if (!renderer->IsValid()) {
			Logging::Error("BaseEngine: Renderer is not valid");
			return;
		}

		// TODO: Move to Renderer.cpp
		glEnable(GL_DEPTH_TEST); // Depth Testing
		glEnable(GL_BLEND); // Transparent Blend
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Call On Start
		//OnEngineInit();

		while(!glfwWindowShouldClose(this->window) && !this->isStopping) {
			// --  IMGUI UPDATE
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			// -- UPDATE
			{
				Time::Update();
				
				Scene::UpdateActiveScene();

				EventSystem::TriggerEvent(BE_EVENT_ENGINE_UPDATE);
			}

			// -- CLEAR SCREEN
			Renderer::ClearBuffer(true, true, true);

			// -- RENDER
			{
				renderer->Render();
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
		//OnEngineDestroy();

		// -- IMGUI TERMINATE
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}
}


