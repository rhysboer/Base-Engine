#include "Application.h"
#include "BaseEngine.h"

#include "BaseEngine.h"

int main() 
{
	Application* app = new Application();
	app->Play();

	delete app;
	return 0;
}