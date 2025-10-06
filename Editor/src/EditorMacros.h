#pragma once
#include "EditorRegistry.h"

#define REGISTER_DETAILS_PAINTER(type)     \
namespace Twisted::Editor                  \
{                                          \
    struct type##Register                  \
    {                                      \
        type##Register()                   \
        {                                  \
            EditorRegistry::GetInstance().RegisterDetailsPainter<type>(); \
        }                                  \
    };                                     \
    static type##Register s_##type##Register; \
}

#define REGISTER_COMPONENT_PAINTER(type)     \
namespace Twisted::Editor                  \
{                                          \
    struct type##Register                  \
    {                                      \
        type##Register()                   \
        {                                  \
            EditorRegistry::GetInstance().RegisterComponentPainter<type>(); \
        }                                  \
    };                                     \
    static type##Register s_##type##Register; \
}



