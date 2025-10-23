#include "Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Twisted
{
	Logger::Logger()
	{
		m_logger = spdlog::stdout_color_st("DAWN");
		m_logger->set_level(spdlog::level::trace);
		spdlog::set_pattern("%^[%T] %n: %v%$");
	}

	Logger& Logger::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = std::make_shared<Logger>();
		}
		return *s_instance;
	}

	bool Logger::Init()
	{
		GetInstance().Info("Logger init success.");
		return true;
	}

	void Logger::Trace(const std::string& str) { m_logger->trace(str); }
	void Logger::Info(const std::string& str) { m_logger->info(str); }
	void Logger::Warn(const std::string& str) { m_logger->warn(str); }
	void Logger::Error(const std::string& str) { m_logger->error(str); }
}