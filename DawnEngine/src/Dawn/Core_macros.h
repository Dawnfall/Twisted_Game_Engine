#pragma once

#include <cassert>

#ifdef DAWN_DEBUG
#define DAWN_ENABLE_ASSERTS
#ifdef DAWN_WINDOWS
#define DAWN_DEBUG_BREAK() __debugbreak()
#else
#error "Dawn Engine only supports Windows!"
#endif
#else
#define DAWN_DEBUG_BREAK()
#endif

#ifdef DAWN_ENABLE_ASSERTS
#define DAWN_DEBUG_ASSERT(...) assert(__VA_ARGS__)
#else 
#define DAWN_DEBUG_ASSERT(...) 
#endif

//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#define _AFX_SECURE_NO_WARNINGS