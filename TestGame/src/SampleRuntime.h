#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

class SampleRuntime :public Twisted::RuntimeBase
{
public:
	SampleRuntime()
	{
		//***********
		// monitor

		Params.window1Title = "Dawn1";
		Params.monitorWidth = 1280;
		Params.monitorHeight = 720;
	}



	//**********
	// game data

	std::string shaderName = "default_shader";
	std::string materialName = "default_material";
	std::string meshName = "test_triangle";
	std::string wallImagePath = "F:/Programiranje/C++/GameEngine/TestGame/Assets/wall.jpg";
	std::string wallImageName = "wall";

	void OnRun(Twisted::Application& app) override;
	void OnInit(Twisted::Application& app)override;
	void OnBeforeRun(Twisted::Application& app) override;

	void LoadEntities(Twisted::Application& app);
	void LoadShaders(Twisted::Application& app);
	void LoadTextures(Twisted::Application& app);
	void LoadMaterials(Twisted::Application& app);
	void LoadMeshes(Twisted::Application& app);
};

