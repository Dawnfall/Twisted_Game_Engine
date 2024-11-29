#include "editorpch.h"
#include "EditorApp.h"

void main()
{

	Twisted::Editor::EditorApp* app = new Twisted::Editor::EditorApp();

	app->Run(nullptr);

	delete app;


}