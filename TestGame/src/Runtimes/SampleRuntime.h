#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

const std::string rootFolder = "F:/Programiranje/C++/GameEngine/TestGame/Assets/";

class SampleRuntime :public Twisted::RuntimeBase
{
public:
	SampleRuntime()
	{
		//***********
		// monitor
		Params.rootFolder = rootFolder;
		Params.window1Title = "Dawn1";
		Params.monitorWidth = 1280;
		Params.monitorHeight = 720;
	}

	//**********
	// game data

	std::string meshName = "cube";
	std::string wallImagePath = "F:/Programiranje/C++/GameEngine/TestGame/Assets/wall.jpg";
	std::string wallImageName = "wall";

	void OnRun(Twisted::Application& app) override;
	void OnInit(Twisted::Application& app)override;
	void OnBeforeRun(Twisted::Application& app) override;

	void LoadEntities(Twisted::Application& app);
	void LoadMaterials(Twisted::Application& app);
};

