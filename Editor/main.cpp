#include "Application/Application.h"

#include "EditorApp/EditorService.h"
#include "GameService.h"
#include "RenderService.h"
#include "AssetsService.h"
#include "Application/TimeService.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
	auto& app = Twisted::Application::GetInstance();

	app.AddService<Twisted::TimeService>(0);
	app.AddService<Twisted::GameService>(100);
	app.AddService<Twisted::RenderService>(200);
	app.AddService<Twisted::AssetsService>(400);
	app.AddService<Twisted::Editor::EditorService>(1000);

	app.Run();

	return 0;
}
