#include "TestApp.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/StandaloneLayer.h"

int main()
{
	Twisted::Application app;
	app.AddLayer<Twisted::StandaloneLayer>();
	app.Run();
}