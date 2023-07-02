#include "Dawn.h"

int main()
{
	Dawn::Application* app = new Dawn::Application();
	app->Init();
	app->Run();
	delete app;

	return 0;
}