#pragma once

#include "pch.h"
#include "EditorWindow.h"

namespace Twisted
{
	class Application;
}

namespace Twisted::Editor
{
	class Editor
	{
	public:
		void Init(Twisted::Application* app);
		void Update(Twisted::Application* app);
		void Render(Twisted::Application* app);
		void Terminate(Twisted::Application* app);

		template<typename T>
		void OpenNewWindow()
		{
			static_assert(std::is_base_of<EditorWindow, T>::value, "T must be derived from EditorWindow");
			for (auto& window : m_openedWindows)
			{
				if (auto castedWindow = dynamic_cast<T*>(window.get()))
					return;
			}
			m_openedWindows.push_back(std::make_unique<T>());
		}

		template<typename T>
		void CloseWindow()
		{
			static_assert(std::is_base_of<EditorWindow, T>::value, "T must be derived from EditorWindow");
			for (int i = 0; i < m_openedWindows.size() - 1; i++)
			{
				if (auto castedWindow = dynamic_cast<T*>(m_openedWindows[i].get()))
				{
					m_openedWindows.erase(m_openedWindows.begin() + i);
					return true;
				}
			}
			return false;
		}

	private:
		std::vector<std::unique_ptr<EditorWindow>> m_openedWindows;
	};
}