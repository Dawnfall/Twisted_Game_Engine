#include "Twisted/Application/Application.h"

#include "EditorApp/EditorWorldService.h"
#include "UI/UIService.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Rendering/RenderService.h"

#include "Twisted/GameProcessor.h"
#include "EditorApp/EditorProcessor.h"


int main()
{
	Twisted::Application editorApplication;
	editorApplication.AddService<Twisted::Editor::EditorWorldService>();
	editorApplication.AddService<Twisted::GameService>();
	editorApplication.AddService<Twisted::WindowsService>();
	editorApplication.AddService<Twisted::Editor::UIService>();
	editorApplication.AddService<Twisted::AssetsService>();
	editorApplication.AddService<Twisted::RenderService>();

	editorApplication.AddProcessor<Twisted::GameProcessor>();
	editorApplication.AddProcessor<Twisted::Editor::EditorProcessor>();

	editorApplication.Run();

	return 0;
}
