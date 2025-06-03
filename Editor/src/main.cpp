#include "editorpch.h"
#include "EditorLayer.h"
#include "Twisted/Application/Application.h"

int main()
{
	Twisted::Application app;
	app.AddLayer<Twisted::Editor::EditorLayer>();
	app.Run();

	return 0;
}