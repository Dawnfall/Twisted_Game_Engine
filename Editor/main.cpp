#include "Twisted/Application/Application.h"

#include "EditorApp/EditorService.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/AssetsLayer/AssetsService.h"

#include "EditorApp/EditorProcessor.h"


int main()
{
	auto& app = Twisted::Application::GetInstance();
	app.AddService<Twisted::Editor::EditorService>();
	app.AddService<Twisted::GameService>();
	app.AddService<Twisted::WindowsService>();
	app.AddService<Twisted::AssetsService>();

	app.AddProcessor<Twisted::Editor::EditorProcessor>();

	app.Run();

	return 0;
}
