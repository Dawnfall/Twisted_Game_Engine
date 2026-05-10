#pragma once

#ifdef _WIN32
    #ifdef SHADER_PARSER_BUILD
        #define SP_API __declspec(dllexport)
    #else
        #define SP_API __declspec(dllimport)
    #endif
#else
    #define SP_API __attribute__((visibility("default")))
#endif
