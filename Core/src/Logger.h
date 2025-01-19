#pragma once
#include <memory>
#include <string>

namespace spdlog
{
	class logger;
}

namespace Twisted
{
	class Logger
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
		static std::shared_ptr<Logger> s_instance;
	};
}

//TODO:... handle release and debug