#include "Dawn.h"
#include "Runtimes/SampleRuntime.h"
#include "StandaloneApp.h"

int main()
{
	Twisted::AppParams params;
	params.rootFolder = "F:/Programiranje/C++/GameEngine/TestGame/Assets/";
	params.windowParams.windowTitle = "Twisted Game Test";
	params.windowParams.monitorWidth = 1280;
	params.windowParams.monitorHeight = 720;

	/*SampleRuntime runtime;
	Twisted::Deploy::StandaloneApp* app = new Twisted::Deploy::StandaloneApp();
	app->Start(params,&runtime);*/

	//delete app;
}