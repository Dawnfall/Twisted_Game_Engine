#include "EditorWorldService.h"

#include "Twisted/Windowing/WindowsService.h"
#include "EditorRegistry.h"

#include "Twisted/Application/Application.h"
#include "UI/ImguiExtensions.h"

#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/World.h"
#include "EditorWorld/EditorCameraSystem.h"

namespace Twisted::Editor
{
	void EditorWorldService::SaveEditor(Window* window)
	{
		GetConfig().SetWindowPos(window->GetPosition());
		GetConfig().SetWindowSize(window->GetSize());
		GetConfig().SaveConfig();
	}


}
