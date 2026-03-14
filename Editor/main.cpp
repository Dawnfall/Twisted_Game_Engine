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
	auto& app = Twisted::Application::GetInstance();
	app.AddService<Twisted::Editor::EditorWorldService>();
	app.AddService<Twisted::GameService>();
	app.AddService<Twisted::WindowsService>();
	app.AddService<Twisted::Editor::UIService>();
	app.AddService<Twisted::AssetsService>();
	app.AddService<Twisted::RenderService>();

	app.AddProcessor<Twisted::GameProcessor>();
	app.AddProcessor<Twisted::Editor::EditorProcessor>();

	app.Run();

	return 0;
}
