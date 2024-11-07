#include "twistedpch.h"
#include "Window_GLFW.h"
#include "Twisted/Rendering/RenderingAPI.h"

namespace Twisted::WindowAPI
{
	void SetCallbacks(GLFWwindow* window, AppBase* callbackPointer)
	{
		glfwSetWindowUserPointer(window, callbackPointer);

		glfwSetWindowCloseCallback(window,
			[](GLFWwindow* window)
			{
				AppBase* app = static_cast<AppBase*>(glfwGetWindowUserPointer(window));
				app->CloseWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window, int newWidth, int newHeight)
			{
				AppBase* app = static_cast<AppBase*>(glfwGetWindowUserPointer(window));
				app->GetWindow()->Width = newWidth;
				app->GetWindow()->Height = newHeight;
				app->WindowResizeEvent.Invoke();
			});
		glfwSetKeyCallback(window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				AppBase* app = static_cast<AppBase*>(glfwGetWindowUserPointer(window));
				app->GetInput().KeyCallback(window, key, scancode, action, mods);
				app->KeyCallbackEvent.Invoke(key);
			});
		glfwSetMouseButtonCallback(window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				AppBase* app = static_cast<AppBase*>(glfwGetWindowUserPointer(window));
				app->GetInput().MouseButtonCallback(window, button, action, mods);
				app->ButtonCallbackEvent.Invoke(button);
			});
	}

	void SwapBuffers(std::shared_ptr<Window> window)
	{
		glfwSwapBuffers(window->Pointer);
	}

	void PollEvents()
	{
		glfwPollEvents();
	}

	std::shared_ptr<Window> CreateNewWindow(const WindowParams& windowParams, AppBase* callbackPointer) //calbackpointer?!?
	{
		//if (m_window != nullptr)
		//{
		//	TWISTED_WARN("CreateNewWindow() failure; window already exists");
		//	return false;
		//}

		GLFWwindow* windowPointer = glfwCreateWindow(windowParams.monitorWidth,windowParams.monitorHeight,windowParams.windowTitle.c_str(), NULL, NULL);;
		if (!windowPointer)
		{
			TWISTED_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return nullptr;
		}

		std::shared_ptr<Window>  newWindow = std::make_shared<Window>(
			windowPointer, windowParams.windowTitle, windowParams.monitorWidth, windowParams.monitorHeight);
		glfwMakeContextCurrent(newWindow->Pointer);
		SetCallbacks(newWindow->Pointer, callbackPointer);

		RenderAPI::InitOpenGL((GLADloadproc)glfwGetProcAddress);

		TWISTED_INFO("Window Created");
		return newWindow;
	}

	bool CloseWindow(Window* window)
	{
		if (window == nullptr)
		{
			TWISTED_WARN("CloseWindow() failure; no window exists");
			return false;
		}

		glfwSetWindowShouldClose(window->Pointer, GLFW_TRUE);
		//glfwDestroyWindow(m_window);

		TWISTED_INFO("Window closed");
		return true;
	}


}