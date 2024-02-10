#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Dawn
{
	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#if DAWN_WINDOWS
	#define DAWN_INFO(...)  Logger::GetLogger()->info(__VA_ARGS__)
	#define DAWN_WARN(...)  Logger::GetLogger()->warn(__VA_ARGS__)
	#define DAWN_ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)
#endif

//TODO:... handle release and debug