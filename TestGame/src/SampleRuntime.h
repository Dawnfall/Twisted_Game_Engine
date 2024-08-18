#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

class SampleRuntime :public Dawn::RuntimeBase
{
public:
	//***********
	// monitor
	unsigned int numberOfMonitors = 1;
	std::string window1Title = "Dawn1";
	unsigned int monitorWidth = 1280;
	unsigned int monitorHeight = 720;

	//**********
	// game data
	std::string shaderName = "default_shader";
	std::string materialName = "default_material";
	std::string meshName = "test_triangle";
	std::string wallImagePath = "F:/Programiranje/C++/GameEngine/TestGame/Assets/wall.jpg";
	std::string wallImageName = "wall";

	void OnRun(Dawn::Application& app) override;
	void OnInit(Dawn::Application& app)override;
	void OnBeforeRun(Dawn::Application& app) override;

	void LoadEntities(Dawn::Application& app);
	void LoadShaders(Dawn::Application& app);
	void LoadTextures(Dawn::Application& app);
	void LoadMaterials(Dawn::Application& app);
	void LoadMeshes(Dawn::Application& app);
};

