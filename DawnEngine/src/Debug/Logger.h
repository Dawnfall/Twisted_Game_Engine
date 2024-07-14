#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Dawn
{
	class Logger
	{
	public:
		static bool Init();

		inline static void Trace(const std::string& str) { s_logger->trace(str); }
		inline static void Info(const std::string& str) { s_logger->info(str); }
		inline static void Warn(const std::string& str) { s_logger->warn(str); }
		inline static void Error(const std::string& str) { s_logger->error(str); }
	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#if DAWN_WINDOWS
	#define DAWN_INFO(...)  Logger::Info(__VA_ARGS__)
	#define DAWN_WARN(...)  Logger::Warn(__VA_ARGS__)
	#define DAWN_ERROR(...) Logger::Error(__VA_ARGS__)
#endif

//TODO:... handle release and debug