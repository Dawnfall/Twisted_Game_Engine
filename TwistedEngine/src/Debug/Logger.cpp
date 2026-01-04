#include "Debug/Logger.h"

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
		static Logger instance; // local static, initialized on first call, thread-safe
		return instance;
	}

	bool Logger::Init()
	{
		GetInstance().Info("Logger init success.");
		return true;
	}

	void Logger::TraceMsg(const std::string& str) { m_logger->log(spdlog::level::trace, str); }
	void Logger::InfoMsg(const std::string& str) { m_logger->log(spdlog::level::info, str); }
	void Logger::WarnMsg(const std::string& str) { m_logger->log(spdlog::level::warn, str); }
	void Logger::ErrorMsg(const std::string& str) { m_logger->log(spdlog::level::err, str); }
}