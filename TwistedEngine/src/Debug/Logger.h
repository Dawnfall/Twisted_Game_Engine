#pragma once

#include "twistedpch.h"
#include "AppCore.h"

namespace Twisted
{
	class TWISTED_API Logger
	{
	public:
		static bool Init();

		inline static void Trace(const std::string& str) { s_logger->trace(str); }
		inline static void Info(const std::string& str) { s_logger->info(str); }
		inline static void Warn(const std::string& str) { s_logger->warn(str); }
		inline static void Error(const std::string& str) { s_logger->error(str); }
	private:
		static SRef<spdlog::logger> s_logger;
	};
}

#if TWISTED_DEBUG
	#define TWISTED_INFO(...)  Logger::Info(__VA_ARGS__)
	#define TWISTED_WARN(...)  Logger::Warn(__VA_ARGS__)
	#define TWISTED_ERROR(...) Logger::Error(__VA_ARGS__)
#endif

//TODO:... handle release and debug