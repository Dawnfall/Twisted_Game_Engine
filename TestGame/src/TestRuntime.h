#pragma once

#include "Dawn.h"
#include <string>
#include <vector>

class TestRuntime :public Twisted::RuntimeBase
{
public:
	//***********
	// monitor
	unsigned int numberOfMonitors = 1;
	std::string window1Title = "Dawn1";
	unsigned int monitorWidth = 1280;
	unsigned int monitorHeight = 720;


	void OnRun(Twisted::Application& app) override;
	void OnInit(Twisted::Application& app)override;
	void OnBeforeRun(Twisted::Application& app) override;

	void TestTransform(Twisted::GameCore& game);

	//void TestEntities(Dawn::Application& app);
};

