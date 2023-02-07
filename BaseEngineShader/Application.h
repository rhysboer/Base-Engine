#pragma once
#include "BaseEngine.h"
#include "GraphManager.h"
#include "Camera.h"
#include "Scene.h"

class Application : public BE::BaseEngine
{
	public:

		
		inline static BE::Camera* GetCamera() { return camera; }
		inline static BE::Scene* GetScene() { return scene; }

	private:
		static BE::Camera* camera;
		static BE::Scene* scene;

		GraphManager* graph = nullptr;

		// Inherited via BaseEngine
		virtual void OnEngineInit() override;
		virtual void OnEngineDestroy() override;

		void OnEngineUpdate() override;
};

