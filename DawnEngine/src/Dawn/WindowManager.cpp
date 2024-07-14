#include "WindowManager.h"

namespace Dawn
{
	void WindowManager::UpdateWindow()
	{
		if (m_window)
		{
			ClearWindow(Color::blue);
			glfwSwapBuffers(m_window->Pointer);
			ProcessEvents();
		}
	}

	bool WindowManager::CreateNewWindow(const WindowData& windowData)
	{
		if (m_window != nullptr)
		{
			DAWN_WARN("Cannot create window; Window already created!");
			return false;
		}

		GLFWwindow* newWindow = glfwCreateWindow(windowData.Width, windowData.Height, windowData.Title.c_str(), NULL, NULL);

		m_window = std::make_unique<Window>(windowData, newWindow);

		glfwMakeContextCurrent(m_window->Pointer); //TODO: not sure if needed
		glfwSetWindowUserPointer(m_window->Pointer, this);

		SetCallbacks();

		RenderContext::Init();
		RenderContext::SetViewPort(m_window->Data.Width, m_window->Data.Height);

		DAWN_INFO("Window Created");
		return true;
	}

	bool WindowManager::DestroyWindow()
	{
		if (m_window != nullptr)
		{
			CloseWindow();
			m_window = nullptr;
			return true;
		}
		return false;
	}

	void WindowManager::ProcessEvents()
	{
		if (m_window)
			glfwPollEvents();
	}

	void WindowManager::ClearWindow(Color color)
	{
		if (m_window)
			RenderContext::Clear(color);//TODO...not deafault
	}

	void WindowManager::CloseWindow()
	{
		if (m_window)
		{
			glfwSetWindowShouldClose(m_window->Pointer, GLFW_TRUE);
			//glfwDestroyWindow(m_window);
		}
	}

	bool WindowManager::IsWindowClosing()
	{
		if (m_window)
			return glfwWindowShouldClose(m_window->Pointer);
	}

	void WindowManager::SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}


	//********************
	// glfw Callbacks

	bool WindowManager::IsKeyPressed(GLFWwindow* window, int key)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS)
			return true;
		return false;
	}
	bool WindowManager::IsKeyReleased(GLFWwindow* window, int key)
	{
		if (glfwGetKey(window, key) == GLFW_RELEASE)
			return true;
		return false;
	}

	void WindowManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//use bind to set callbacks
		//...... TODO:.....

	}


	void WindowManager::SetCallbacks()
	{
		glfwSetWindowCloseCallback(m_window->Pointer,
			[](GLFWwindow* window)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				thisManager->closeWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(m_window->Pointer,
			[](GLFWwindow* window,int newWidth,int newHeight)
			{
				WindowManager* thisManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
				//thisManager->m_window->Data.Width = newWidth;
				//thisManager->m_window->Data.Height = newHeight;
			});
		
	}

}