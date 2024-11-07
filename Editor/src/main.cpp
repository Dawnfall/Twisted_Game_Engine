#include "editorpch.h"
#include "EditorApp.h"

void main()
{
	Twisted::AppParams params;
	params.rootFolder = "F:/Programiranje/C++/GameEngine/TestGame/Assets/";
	params.windowParams.windowTitle = "Twisted Game Test";
	params.windowParams.monitorWidth = 1280;
	params.windowParams.monitorHeight = 720;

	Twisted::Editor::EditorApp* app = new Twisted::Editor::EditorApp();

	app->Start(params,nullptr);

	delete app;


}