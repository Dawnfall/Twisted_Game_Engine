#pragma once
#include "API.h"

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

enum class ShaderPropertyType { Float, Double, Int, Vec2, Vec3, Vec4, Color, Texture };

using Vec2Prop = std::array<float, 2>;
using Vec3Prop = std::array<float, 3>;
using Vec4Prop = std::array<float, 4>;

struct SP_API ShaderProperty
{
    ShaderPropertyType type;
    std::string        shaderName;
    std::string        editorName;
};

struct SP_API SPReflection
{
    std::vector<ShaderProperty> properties;
    bool        ok = true;
    std::string errorMessage;
};

SP_API SPReflection ParseShaderRef(std::string_view refBlock);
