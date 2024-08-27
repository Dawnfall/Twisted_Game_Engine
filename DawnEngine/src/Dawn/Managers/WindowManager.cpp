#include "WindowManager.h"
#include "Dawn/Rendering/OpenGL/Render_OpenGL.h"
#include "Debug/Logger.h"
#include <glad/glad.h>

namespace Dawn
{
	bool WindowManager::CreateNewWindow(const std::string& title, unsigned int width, unsigned int height)
	{
		if (m_windows.size() > 2)
		{
			DAWN_WARN("CreateNewWindow() failure; 2 windows max");
			return false;
		}

		GLFWwindow* windowPointer;
		if (m_windows.size() == 0)
			windowPointer = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		else
			windowPointer = glfwCreateWindow(width, height, title.c_str(), NULL, m_windows[0]->Pointer);
		if (!windowPointer)
		{
			DAWN_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return false;
		}

		std::shared_ptr<Window> newWindow = std::make_shared<Window>(windowPointer, m_nextID++, title, width, height);
		m_windows.emplace_back(newWindow);
		glfwMakeContextCurrent(newWindow->Pointer);
		glfwSetWindowUserPointer(newWindow->Pointer, this);

		if (m_windows.size() == 1) //all windows share same context
		{
			Render_OpenGL::InitOpenGL((GLADloadproc)glfwGetProcAddress);
		}

		SetCallbacks();

		DAWN_INFO("Window Created");
		return true;
	}

	std::shared_ptr<Window> WindowManager::GetWindow(int id)
	{
		for (auto& window : m_windows)
		{
			if (window->ID == id)
				return window;
		}
		return nullptr;
	}

	bool WindowManager::CloseWindow(unsigned int id)
	{
		for (int i = 0; i < m_windows.size(); i++)
		{
			auto& window = m_windows[i];
			if (window->ID == id)
			{
				glfwSetWindowShouldClose(window->Pointer, GLFW_TRUE);
				//glfwDestroyWindow(m_window);

				m_windows.erase(std::next(m_windows.begin(), i));
				DAWN_INFO("Window closed");
				return true;
			}
		}

		DAWN_WARN("CloseWindow() failure; no window with id: " + id);
		return false;
	}

	std::shared_ptr<Window> WindowManager::GetWindow(GLFWwindow* pointer)
	{
		for (auto& window : m_windows)
			if (window->Pointer == pointer)
				return window;
		return nullptr;
	}

	void WindowManager::ProcessEvents()
	{
		if (m_windows.size() > 0)
		{
			m_inputManager.Clear();
			glfwPollEvents();
		}
	}

	void WindowManager::UpdateWindows()
	{
		for (auto& window : m_windows)
		{
			glfwSwapBuffers(window->Pointer);
		}
		ProcessEvents();
	}

	//********************
	// glfw Callbacks


	void WindowManager::SetCallbacks()
	{
		for (auto& window : m_windows)
		{
			glfwSetWindowCloseCallback(window->Pointer,
				[](GLFWwindow* window)
				{
					WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
					thisManager->closeWindowEvent.Invoke();
				});
			glfwSetWindowSizeCallback(window->Pointer,
				[](GLFWwindow* window, int newWidth, int newHeight)
				{
					WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
					thisManager->GetWindow(window)->Width = newWidth;
					thisManager->GetWindow(window)->Height = newHeight;
				});
			glfwSetKeyCallback(window->Pointer,
				[](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
					thisManager->m_inputManager.KeyCallback(window, key, scancode, action, mods);
				});
			glfwSetMouseButtonCallback(window->Pointer,
				[](GLFWwindow* window, int button, int action, int mods)
				{
					WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
					thisManager->m_inputManager.MouseButtonCallback(window, button, action, mods);
				});
		}
	}
}