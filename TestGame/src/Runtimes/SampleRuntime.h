#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

class SampleRuntime :public Twisted::RuntimeBase
{
public:
	//**********
	// game data

	std::string meshName = "cube";
	std::string wallImagePath = "F:/Programiranje/C++/GameEngine/TestGame/Assets/wall.jpg";
	std::string wallImageName = "wall";

	void OnRun(Twisted::AppBase* app) override;
	void OnInit(Twisted::AppBase* app)override;
	void OnBeforeRun(Twisted::AppBase* app) override;

	void LoadEntities(Twisted::AppBase* app);
	void LoadMaterials(Twisted::AppBase* app);
};

