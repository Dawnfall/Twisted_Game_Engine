#pragma once
#include "Logger.h"
#include <memory>

//************
// DLLimport / DLLexport

#ifdef TWISTED_WINDOWS
	#ifdef TWISTED_ENGINE
		#define TWISTED_API __declspec(dllexport)
	#else
		#define TWISTED_API __declspec(dllimport)
	#endif
#else
	#error "Twisted Engine only supports Windows!"
#endif

//**************
// Asserts

#ifdef TWISTED_DEBUG
	#define TWISTED_ENABLE_ASSERTS
	#ifdef TWISTED_WINDOWS
		#define TWISTED_DEBUG_BREAK() __debugbreak()
	#else
		#define TWISTED_DEBUG_BREAK()	
	#endif
#else
	#define TWISTED_DEBUG_BREAK()
#endif

#ifdef TWISTED_ENABLE_ASSERTS
	#define TWISTED_DEBUG_ASSERT(...) assert(__VA_ARGS__)
#else 
	#define TWISTED_DEBUG_ASSERT(...) 
#endif

#if TWISTED_DEBUG
#define TWISTED_INFO(...)  Logger::GetInstance().Info(__VA_ARGS__)
#define TWISTED_WARN(...)  Logger::GetInstance().Warn(__VA_ARGS__)
#define TWISTED_ERROR(...) Logger::GetInstance().Error(__VA_ARGS__)
#endif

//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#define _AFX_SECURE_NO_WARNINGS

template<typename T>
using SRef = std::shared_ptr<T>;

template<typename T>
using URef = std::unique_ptr<T>;
