#include "Dawn.h"
#include "TestRuntime.h"
#include "SampleRuntime.h"

int main()
{
	//TestRuntime runtime;
	SampleRuntime runtime;
	Dawn::Application* app = new Dawn::Application();
	app->Start(runtime);

	delete app;

	return 0;
}