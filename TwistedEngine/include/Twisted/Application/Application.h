#pragma once
#include "AppCore.h"

#include <memory>
#include <vector>
#include <chrono>
#include "Service.h"
#include "Processor.h"
#include "TimeService.h"

namespace Twisted
{
	struct FrameInfo
	{
		float deltaTime;
	};

	class TimeService;
	class TWISTED_API Application
	{
	public:
		Application() {}
		Application& operator=(const Application& other) = delete;
		Application(const Application& other) = delete;

		void Run();
		void Stop() { m_isRunning = false; }

		template<typename T>
		T* AddService()
		{
			static_assert(std::is_base_of<Service, T>::value, "T must inherit from Service class");

			if (T* service = GetService<T>(); service)
				return service;

			m_services.emplace_back(std::make_unique<T>(this));
			return static_cast<T*>(m_services.back().get());
		}
		template<typename T>
		T* AddProcessor()
		{
			static_assert(std::is_base_of<Processor, T>::value, "T must inherit from Processor class");
			m_processors.emplace_back(std::make_unique<T>(this));
			return static_cast<T*>(m_processors.back().get());
		}

		template<typename T>
		T* GetService()
		{
			static_assert(std::is_base_of<Service, T>::value, "T must inherit from Service class");
			for (auto& service : m_services)
			{
				if (auto ptr = dynamic_cast<T*>(service.get()))
					return ptr;
			}
			return nullptr;
		}
		template<typename T>
		T* GetProcessor()
		{
			static_assert(std::is_base_of<Processor, T>::value, "T must inherit from Processor class");
			for (auto& processor : m_processors)
			{
				if (auto ptr = dynamic_cast<T*>(processor.get()))
					return ptr;
			}
			return nullptr;
		}
	protected:
		std::vector<URef<Service>> m_services;
		std::vector<URef<Processor>> m_processors;
		TimeService* m_timeService = nullptr;

		bool m_isRunning = false;
	};


}

