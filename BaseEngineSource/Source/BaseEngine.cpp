#include "BaseEngine.h"
#include "Input.h"
#include "Logging.h"
#include "TimeManager.h"
#include "Gizmos.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "PhysicsManager.h"
#include "BEGlobal.h"
#include "EventSystem.h"
#include "Renderer.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifdef BE_DEBUG
#include "Oracle.h"
#endif

void Callback_WindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	glm::vec2 size = glm::vec2(width, height);
	BE::EventSystem::TriggerEvent(BE_EVENT_WINDOW_RESIZE, &size);
}

void Callback_Error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(type == GL_DEBUG_TYPE_ERROR)
		BE_ERROR("GL CALLBACK: ** GL ERROR ** type = 0x%x, severity = 0x%x, message = %s", type, severity, message);
}

namespace BE {
	BaseEngine* BaseEngine::engine = nullptr;

	BaseEngine::BaseEngine(const EngineDesc& desc) : window(nullptr), renderer(nullptr)
	{
		BE_ASSERT(engine == nullptr, "Can not create another engine when one already exists");
		BE_ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc.OpenGL_GetVersionMajor());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc.OpenGL_GetVersionMinor());
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		engine = this;
		engine->window = glfwCreateWindow(desc.width, desc.height, desc.name, NULL, NULL);

		BE_ASSERT(engine->window != nullptr, "Failed to create window");

		glfwMakeContextCurrent(engine->window);
		//glfwSwapInterval(0);

		BE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to start glad");

		physicsManager = new BE::PhysicsManager();

		EventSystem::CreateEvent(BE_EVENT_WINDOW_RESIZE);
		EventSystem::CreateEvent(BE_EVENT_WINDOW_MINIMISE);
		EventSystem::CreateEvent(BE_EVENT_ENGINE_UPDATE);
	}

	BaseEngine::~BaseEngine()
	{ 
		delete renderer;
		delete physicsManager;
	}

	void BaseEngine::CreateEngine(const RenderDesc& renderDesc) 
	{
		// Initiate
		BE::Renderer::RegisterGlobalTextures(renderDesc.globalTextures);
		ShaderManager::Init();
		BEGlobal::Init();

		glViewport(0, 0, desc.width, desc.height);
		
		engine->renderer = new Renderer(renderDesc);

		// -- Register Callbacks Here
		glfwSetWindowSizeCallback(engine->window, Callback_WindowResize);
#ifdef _DEBUG 
		if (desc.OpenGL_GetVersionMajor() >= 4 && desc.OpenGL_GetVersionMinor() >= 3) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(Callback_Error, 0);
		}
#endif

		Input::SetCallbacks(engine->window);

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(engine->window, true);
		ImGui_ImplOpenGL3_Init(desc.OpenGL_GetGLSLShaderVersion()); 
		ImGui::StyleColorsDark();
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

#ifdef BE_DEBUG
		Oracle::Init();
#endif // BE_DEBUG

		// Call On Start
		//OnEngineInit();

		Time::Update();
		physicsManager->Start();

		while(!glfwWindowShouldClose(this->window) && !this->isStopping) {
			// --  IMGUI UPDATE
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			OnFrameUpdate();

			// -- UPDATE
			{
				Time::Update();
				
				physicsManager->Update();

				Scene::UpdateActiveScene();

				//EventSystem::TriggerEvent(BE_EVENT_ENGINE_UPDATE);
			}

			// -- CLEAR SCREEN
			Renderer::ClearBuffer(true, true, true);

			// -- RENDER
			{
				renderer->Render();
			}

#ifdef BE_DEBUG
			Oracle::Draw();
#endif // BE_DEBUG

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


		ShaderManager::Destroy();
		// -- DESTROY ENGINE
		//OnEngineDestroy();

		// -- IMGUI TERMINATE
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		window = nullptr;
	}
}


