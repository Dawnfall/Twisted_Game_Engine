#include "twistedpch.h"
#include "Logger.h"


namespace Twisted
{
	std::shared_ptr<spdlog::logger> Logger::s_logger;

	bool Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("DAWN");
		s_logger->set_level(spdlog::level::trace);

		TWISTED_INFO("Logger init success.");
		return true;
	}
}