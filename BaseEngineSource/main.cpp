#include "Application.h"
#include "BaseEngine.h"

int main() 
{
	auto engineDesc = BE::EngineDesc();
	engineDesc.name = "Engine Title";

	Application* app = new Application(engineDesc);
	app->Run();

	delete app;
	return 0;
}