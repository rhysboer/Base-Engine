#include "Application.h"
#include "BaseEngine.h"

int main() {

	Application* app = new Application();
	app->CreateEngine("New Title", 1280, 720);
	app->StartEngine();

	delete app;

	return 0;
}