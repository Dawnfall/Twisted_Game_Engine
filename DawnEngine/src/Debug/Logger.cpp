#include "Logger.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Dawn
{
	std::shared_ptr<spdlog::logger> Logger::s_logger;

	bool Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("DAWN");
		s_logger->set_level(spdlog::level::trace);

		DAWN_INFO("Logger init success.");
		return true;
	}
}