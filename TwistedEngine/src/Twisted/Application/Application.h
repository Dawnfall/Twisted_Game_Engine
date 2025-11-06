#pragma once
#include "AppCore.h"
#include "Layer.h"
#include "TimeManager.h"

#include <memory>
#include <vector>

namespace Twisted
{
	class TWISTED_API Application
	{
	public:
		Application() {}
		Application& operator=(const Application& other) = delete;
		Application(const Application& other) = delete;

		void Run();
		void Stop() { m_isRunning = false; }

		TimeManager& GetTime() { return m_time; }
		const TimeManager& GetTime()const { return m_time; }

		template<typename T>
		T* AddLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Layer must inherit from Layer class");
			m_layers.emplace_back(std::make_unique<T>(this));
			return static_cast<T*>(m_layers.back().get());
		}
		template<typename T>
		T* GetLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Layer must inherit from Layer class");
			for (auto& layer : m_layers)
			{
				if (auto ptr = dynamic_cast<T*>(layer.get()))
					return ptr;
			}
			return nullptr;
		}

	protected:

		virtual void OnCreate() = 0;
		virtual void OnBeforeRun() = 0;
		virtual void OnFrame() = 0;
		virtual void OnTerminate() = 0;

		TimeManager m_time;

		std::vector<URef<Layer>> m_layers;
		bool m_isRunning = false;
	};


}