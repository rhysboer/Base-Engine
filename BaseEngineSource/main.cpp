#include "Application.h"

int main() {
	Application* app = new Application();
	app->CreateEngine("Base Engine", 1280, 720);
	app->Run();

	delete app;

	return 0;
}