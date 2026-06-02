#pragma once
#include "AppCore.h"
#include "Utils/Utils.h"

#include <memory>
#include <string>
#include <spdlog/common.h>

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
		void LogAtMsg(spdlog::source_loc loc, spdlog::level::level_enum lvl, const std::string& msg);

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

		template<typename... Args>
		void LogAt(spdlog::source_loc loc, spdlog::level::level_enum lvl, Args&&... args)
		{
			std::string msg = Utils::buildString(std::forward<Args>(args)...);
			LogAtMsg(loc, lvl, msg);
		}

		static Logger& GetInstance();
		static bool Init();
	private:
		std::shared_ptr<spdlog::logger> m_logger = nullptr;
	};
}

#ifdef TWISTED_DEBUG
#define LOGGER_INIT() ::Twisted::Logger::Init()
#define TWISTED_TRACE(...) ::Twisted::Logger::GetInstance().LogAt({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, __VA_ARGS__)
#define TWISTED_INFO(...)  ::Twisted::Logger::GetInstance().LogAt({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info,  __VA_ARGS__)
#define TWISTED_WARN(...)  ::Twisted::Logger::GetInstance().LogAt({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn,  __VA_ARGS__)
#define TWISTED_ERROR(...) ::Twisted::Logger::GetInstance().LogAt({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err,   __VA_ARGS__)
#else
#define LOGGER_INIT()
#define TWISTED_TRACE(...)
#define TWISTED_INFO(...)
#define TWISTED_WARN(...)
#define TWISTED_ERROR(...)

#define TWISTED_WARN_TEST(...)
#endif


//TODO:... handle release and debug

#ifdef TWISTED_DEBUG
#define TWISTED_ASSERT(condition, msg)                                          \
    do {                                                                        \
        if (!(condition)) {                                                     \
            TWISTED_ERROR("Assertion failed: {} | {}", #condition, msg);        \
            TWISTED_DEBUG_BREAK();                                               \
            std::abort();                                                       \
        }                                                                       \
    } while(0)
#else
#define TWISTED_ASSERT(condition, msg)
#endif
