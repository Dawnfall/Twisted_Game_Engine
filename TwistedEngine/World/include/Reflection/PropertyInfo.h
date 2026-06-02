#pragma once

#include <vector>
#include <functional>
#include <string>
#include "Utils/GlmUtils.h"

namespace Twisted
{
    enum class PropertyType
    {
        Float,
        Int,
        Bool,
        String,
        Vec2f,
        Vec3f,
        Vec4f,
        Quat,
    };

    struct PropertyInfo
    {
        const char*  displayName;
        const char*  yamlKey;       // matches field name — used by auto-serialization (Phase 3)
        PropertyType type;
        std::function<void*(void*)> accessor;   // (ComponentPtr) → &field
    };

    template<typename T>
    std::vector<PropertyInfo>& GetTypeProperties()
    {
        static std::vector<PropertyInfo> s_list;
        return s_list;
    }
}

// Maps C++ types to PropertyType — add specializations here as new types are needed
template<typename T> constexpr Twisted::PropertyType PropertyTypeOf() = delete;
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<float>()          { return Twisted::PropertyType::Float;  }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<int>()            { return Twisted::PropertyType::Int;    }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<bool>()           { return Twisted::PropertyType::Bool;   }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<std::string>()    { return Twisted::PropertyType::String; }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<Vec2f>() { return Twisted::PropertyType::Vec2f;  }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<Vec3f>() { return Twisted::PropertyType::Vec3f;  }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<Vec4f>() { return Twisted::PropertyType::Vec4f;  }
template<> inline constexpr Twisted::PropertyType PropertyTypeOf<Quat>()  { return Twisted::PropertyType::Quat;   }
