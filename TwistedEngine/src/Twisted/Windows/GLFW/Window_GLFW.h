#pragma once

#include "twistedpch.h"
#include "Twisted/Windows/Window.h" 
#include "Twisted/AppBase.h"
#include "Twisted/AppParams.h"

namespace Twisted::WindowAPI
{
	void SetCallbacks(GLFWwindow* window, AppBase* callbackPointer);
	void SwapBuffers(std::shared_ptr<Window> window);
	void PollEvents();

	std::shared_ptr<Window> CreateNewWindow(const WindowParams& windowParams, AppBase* callbackPointer); //calbackpointer?!?

	bool CloseWindow(Window* window);
}