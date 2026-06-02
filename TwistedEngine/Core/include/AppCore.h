#pragma once
#include <memory>
#include <filesystem>
#include <cstdlib>


//************
// DLLimport / DLLexport
// Core is built as a static library — TWISTED_API is empty.
// Define TWISTED_SHARED_LIB before including this header to use dllexport/dllimport.

#ifdef TWISTED_SHARED_LIB
	#ifdef _WIN32
		#ifdef TWISTED_ENGINE
			#define TWISTED_API __declspec(dllexport)
		#else
			#define TWISTED_API __declspec(dllimport)
		#endif
	#else
		#define TWISTED_API __attribute__((visibility("default")))
	#endif
#else
	#define TWISTED_API
#endif

//**************
// Asserts

#ifdef TWISTED_DEBUG
	#define TWISTED_ENABLE_ASSERTS
	#ifdef _WIN32
		#define TWISTED_DEBUG_BREAK() __debugbreak()
	#else
		#include <signal.h>
		#define TWISTED_DEBUG_BREAK() raise(SIGTRAP)
	#endif
#else
	#define TWISTED_DEBUG_BREAK()
#endif

#ifdef TWISTED_ENABLE_ASSERTS
	#define TWISTED_DEBUG_ASSERT(...) assert(__VA_ARGS__)
#else
	#define TWISTED_DEBUG_ASSERT(...)
#endif



//template<typename T, typename... Ts>
//constexpr bool none_are_same() {
//	return (!std::is_same<T, Ts>::value && ...);
//}

//static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
//#define TWISTED_ASSERT_NOT_OF_TYPE(EXCLUDED_TYPE, ...)
//    static_assert(none_are_same<EXCLUDED_TYPE, __VA_ARGS__>(),
//                  "Component types must not be " #EXCLUDED_TYPE)


//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#define _AFX_SECURE_NO_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL

template<typename T>
using SRef = std::shared_ptr<T>;

template<typename T>
using URef = std::unique_ptr<T>;
