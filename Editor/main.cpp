#include "Application/Application.h"

#include "EditorApp/EditorService.h"
#include "GameService.h"
#include "WindowsService.h"
#include "AssetsService.h"
#include "Application/TimeService.h"


int main()
{
	auto& app = Twisted::Application::GetInstance();

	app.AddService<Twisted::TimeService>(0);
	app.AddService<Twisted::GameService>(100);
	app.AddService<Twisted::WindowsService>(200);
	app.AddService<Twisted::RenderService>(300);
	app.AddService<Twisted::AssetsService>(400);
	app.AddService<Twisted::Editor::EditorService>(1000);

	app.Run();

	return 0;
}
