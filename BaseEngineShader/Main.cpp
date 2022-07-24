#include "Application.h"


int main() {

	Application* app = new Application();
	app->CreateEngine("Base Engine Shader Forge", 1280, 720);
	app->StartEngine();

	delete app;
	return 0;
}