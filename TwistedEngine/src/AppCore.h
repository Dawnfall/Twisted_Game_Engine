#pragma once

#include "twistedpch.h"

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

//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#define _AFX_SECURE_NO_WARNINGS

using EntityID = entt::entity;
constexpr entt::entity NullEntity = entt::null;