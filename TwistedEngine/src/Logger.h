#pragma once
#include <memory>
#include <string>
#include "AppCore.h"

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

		void Trace(const std::string& str);
		void Info(const std::string& str);
		void Warn(const std::string& str);
		void Error(const std::string& str);

		static Logger& GetInstance();
		static bool Init();
	private:
		std::shared_ptr<spdlog::logger> m_logger = nullptr;
		static inline std::shared_ptr<Logger> s_instance;
	};
}

#if TWISTED_DEBUG
	#define TWISTED_INFO(...)  Twisted::Logger::GetInstance().Info(__VA_ARGS__)
	#define TWISTED_WARN(...)  Twisted::Logger::GetInstance().Warn(__VA_ARGS__)
	#define TWISTED_ERROR(...) Twisted::Logger::GetInstance().Error(__VA_ARGS__)
#else
	#define TWISTED_INFO(...) 
	#define TWISTED_WARN(...) 
	#define TWISTED_ERROR(...) 

	#define TWISTED_WARN_TEST(...)
#endif


//TODO:... handle release and debug