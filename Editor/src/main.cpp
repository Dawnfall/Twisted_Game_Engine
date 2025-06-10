#include "Twisted/Application/Application.h"
#include "EditorRuntime.h"

int main()
{
	Twisted::Editor::EditorRuntime runtime;
	Twisted::Application app(&runtime);
	app.Run();

	return 0;
}