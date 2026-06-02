#pragma once
#include "API.h"
#include "SPReflection.h"

#include <optional>
#include <string>
#include <string_view>

struct SP_API ShaderData
{
    std::optional<std::string> vert;
    std::optional<std::string> frag;
    std::optional<std::string> geom;
    SPReflection               reflection;
    bool                       ok = true;
    std::string                errorMessage;
};

// Runs the full pipeline: split blocks → expand #imports → parse REF.
SP_API ShaderData ProcessShaderFile(std::string_view source, const std::string& baseDirectory = {});
