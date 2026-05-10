#pragma once
#include "AppCore.h"
#include "Utils/Utils.h"

#include <memory>
#include <string>

namespace spdlog
{
	class logger;
}

namespace Twisted
{
	class TWISTED_API Logger
	{
	public:
		Logger();

		void TraceMsg(const std::string& str);
		void InfoMsg(const std::string& str);
		void WarnMsg(const std::string& str);
		void ErrorMsg(const std::string& str);

		// Variadic logging (no fmt)
		template<typename... Args>
		void Trace(Args&&... args) 
		{ 
			std::string msg = Utils::buildString(std::forward<Args>(args)...);
			TraceMsg(msg);
		}

		template<typename... Args>
		void Info(Args&&... args)
		{
			std::string msg = Utils::buildString(std::forward<Args>(args)...);
			InfoMsg(msg);
		}

		template<typename... Args>
		void Warn(Args&&... args)
		{
			std::string msg = Utils::buildString(std::forward<Args>(args)...);
			WarnMsg(msg);
		}

		template<typename... Args>
		void Error(Args&&... args)
		{
			std::string msg = Utils::buildString(std::forward<Args>(args)...);
			ErrorMsg(msg);
		}


		static Logger& GetInstance();
		static bool Init();
	private:
		std::shared_ptr<spdlog::logger> m_logger = nullptr;
	};
}

#if TWISTED_DEBUG
#define LOGGER_INIT() ::Twisted::Logger::Init()
#define TWISTED_TRACE(...) ::Twisted::Logger::GetInstance().Trace(__VA_ARGS__)
#define TWISTED_INFO(...)  ::Twisted::Logger::GetInstance().Info(__VA_ARGS__)
#define TWISTED_WARN(...)  ::Twisted::Logger::GetInstance().Warn(__VA_ARGS__)
#define TWISTED_ERROR(...) ::Twisted::Logger::GetInstance().Error(__VA_ARGS__)
#else
#define LOGGER_INIT()
#define TWISTED_TRACE(...)
#define TWISTED_INFO(...) 
#define TWISTED_WARN(...) 
#define TWISTED_ERROR(...) 

#define TWISTED_WARN_TEST(...)
#endif


//TODO:... handle release and debug