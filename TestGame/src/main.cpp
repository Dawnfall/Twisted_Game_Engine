#include "Dawn.h"
#include "Runtimes/SampleRuntime.h"

int main()
{
	//TestRuntime runtime;
	SampleRuntime runtime;
	Twisted::Application* app = new Twisted::Application();
	app->Start(runtime);

	delete app;

	return 0;
}