#pragma once
#include "Dawn.h"

#include <string>

class TestRuntime :public Dawn::RuntimeBase
{
public:
	unsigned int numberOfMonitors = 1;
	std::string window1Title = "Dawn1";
	std::string window2Title = "Dawn2";
	unsigned int width = 1280;
	unsigned int height = 720;
	
	//Dawn::Mesh triangle;
	Dawn::ShaderData shaderData;

	TestRuntime() :
		shaderData("default shader", Dawn::Collections::vertexShaderCode, Dawn::Collections::fragmentShaderCode)
	{}

	void OnInit(Dawn::Application& app)override 
	{
		app.ResourceManager.AddResource(shaderData);

		app.WindowManager.CreateNewWindow(window1Title,width,height);
		app.WindowManager.closeWindowEvent.AddListener([&app]() { app.Stop(); });

	}

	void OnBeforeRun(Dawn::Application& app) override
	{

	}

	void OnRun(Dawn::Application& app) override
	{

	}
};

