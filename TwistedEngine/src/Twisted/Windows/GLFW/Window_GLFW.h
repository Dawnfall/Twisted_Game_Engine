#pragma once

#include "twistedpch.h"
#include "Twisted/Windows/Window.h" 
#include "Twisted/AppBase.h"
#include "Twisted/AppParams.h"

#include "AppCore.h"

namespace Twisted::WindowAPI
{
	void TWISTED_API SetCallbacks(GLFWwindow* window, AppBase* callbackPointer);
	void TWISTED_API SwapBuffers(std::shared_ptr<Window> window);
	void TWISTED_API PollEvents();

	std::shared_ptr<Window> TWISTED_API CreateNewWindow(const WindowParams& windowParams, AppBase* callbackPointer); //calbackpointer?!?

	bool TWISTED_API CloseWindow(Window* window);
}