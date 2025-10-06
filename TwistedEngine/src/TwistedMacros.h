#pragma once

#define REGISTER_IMPORTER(type)            \
namespace Twisted                          \
{                                          \
    struct type##Register                  \
    {                                      \
        type##Register()                   \
        {                                  \
            ImporterRegistry::GetInstance().RegisterImporter<type>(); \
        }                                  \
    };                                     \
    static type##Register s_##type##Register; \
}