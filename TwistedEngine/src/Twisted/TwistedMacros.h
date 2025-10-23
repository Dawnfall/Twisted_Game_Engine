#pragma once
#include "Twisted/Gameing/WorldSerializer.h"

#define REGISTER_IMPORTER(type)                                             \
namespace Twisted                                                           \
{                                                                           \
    struct type##_Importer_Register                                         \
    {                                                                       \
        type##_Importer_Register()                                          \
        {                                                                   \
            AssetImporterRegistry::GetInstance().RegisterImporter<type>();  \
        }                                                                   \
    };                                                                      \
    static type##_Importer_Register s_##type##_Importer_Register;           \
}

#define REGISTER_COMPONENT(T,name)                                          \
namespace Twisted                                                           \
{                                                                           \
    template<>                                                              \
    constexpr const char* GetComponentName<T>() { return name; }            \
    struct T##_Component_Register                                           \
    {                                                                       \
        T##_Component_Register ()                                           \
        {                                                                   \
            WorldSerializer::GetInstance().RegisterComponent<T>();          \
        }                                                                   \
    };                                                                      \
    static T##_Component_Register s_##T##_Component_Register;               \
}