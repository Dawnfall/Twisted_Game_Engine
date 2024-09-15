#include "pch.h"

#include "WindowManager.h"
#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Debug/Logger.h"
#include <glad/glad.h>

namespace Twisted
{
	bool WindowManager::CreateNewWindow(const std::string& title, unsigned int width, unsigned int height)
	{
		if (m_window != nullptr)
		{
			TWISTED_WARN("CreateNewWindow() failure; window already exists");
			return false;
		}


		GLFWwindow* windowPointer = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);;
		if (!windowPointer)
		{
			TWISTED_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return false;
		}

		m_window = std::make_shared<Window>(windowPointer, title, width, height);
		glfwMakeContextCurrent(m_window->Pointer);
		glfwSetWindowUserPointer(m_window->Pointer, this);

		Render_OpenGL::InitOpenGL((GLADloadproc)glfwGetProcAddress);

		SetCallbacks();

		TWISTED_INFO("Window Created");
		return true;
	}

	bool WindowManager::CloseWindow()
	{
		if (m_window == nullptr)
		{
			TWISTED_WARN("CloseWindow() failure; no window exists");
			return false;
		}

		glfwSetWindowShouldClose(GetWindow()->Pointer, GLFW_TRUE);
		//glfwDestroyWindow(m_window);
		m_window = nullptr;

		TWISTED_INFO("Window closed");
		return true;
	}


	void WindowManager::ProcessEvents()
	{
		if (GetWindow() == nullptr)
			return;

		m_inputManager.Clear();
		glfwPollEvents();
	}

	void WindowManager::UpdateWindows()
	{
		if (GetWindow() == nullptr)
			return;

		glfwSwapBuffers(GetWindow()->Pointer);
		ProcessEvents();
	}

	//********************
	// glfw Callbacks


	void WindowManager::SetCallbacks()
	{
		glfwSetWindowCloseCallback(GetWindow()->Pointer,
			[](GLFWwindow* window)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				thisManager->closeWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(GetWindow()->Pointer,
			[](GLFWwindow* window, int newWidth, int newHeight)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				thisManager->GetWindow()->Width = newWidth;
				thisManager->GetWindow()->Height = newHeight;
			});
		glfwSetKeyCallback(GetWindow()->Pointer,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				thisManager->m_inputManager.KeyCallback(window, key, scancode, action, mods);
			});
		glfwSetMouseButtonCallback(GetWindow()->Pointer,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				thisManager->m_inputManager.MouseButtonCallback(window, button, action, mods);
			});

	}
}