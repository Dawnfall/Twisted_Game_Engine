#pragma once
#include "AppCore.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <chrono>
#include "Service.h"
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
		static Application& GetInstance()
		{
			static Application app;
			return app;
		}

		Application& operator=(const Application& other) = delete;
		Application(const Application& other) = delete;

		void Run();
		void Stop() { m_isRunning = false; }

		template<typename T>
		T* AddService(int priority)
		{
			static_assert(std::is_base_of<Service, T>::value, "T must inherit from Service class");

			if (T* service = GetService<T>(); service)
				return service;

			m_services.emplace_back(std::make_unique<T>(this,priority));
			std::sort(m_services.begin(), m_services.end(), [](const URef<Service>& a, const URef<Service>& b) {
				return a->GetPriority() > b->GetPriority();
			});
			return GetService<T>();
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
	protected:
		Application() {}

		std::vector<URef<Service>> m_services;

		bool m_isRunning = false;
	};


}

