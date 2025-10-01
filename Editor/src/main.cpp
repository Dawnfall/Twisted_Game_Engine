#include "Twisted/Application/Application.h"
#include "EditorRuntime.h"

#include "Utils/Utils.h"
#include <iostream>

int main()
{
	Twisted::Editor::EditorRuntime runtime;
	Twisted::Application app(&runtime);
	app.Run();

	return 0;
}