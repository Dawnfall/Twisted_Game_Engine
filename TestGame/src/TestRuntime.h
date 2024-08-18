#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

class TestRuntime :public Dawn::RuntimeBase
{
public:
	//***********
	// monitor
	unsigned int numberOfMonitors = 1;
	std::string window1Title = "Dawn1";
	unsigned int monitorWidth = 1280;
	unsigned int monitorHeight = 720;


	void OnRun(Dawn::Application& app) override;
	void OnInit(Dawn::Application& app)override;
	void OnBeforeRun(Dawn::Application& app) override;

	void TestTransform(Dawn::GameCore& game);

	//void TestEntities(Dawn::Application& app);
};

