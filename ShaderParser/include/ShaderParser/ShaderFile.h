#pragma once
#include "ShaderParser.h"

#include <optional>
#include <string>
#include <string_view>

struct SP_API ShaderBlocks
{
    std::optional<std::string> vert;
    std::optional<std::string> frag;
    std::optional<std::string> geom;
    std::optional<std::string> ref;
    bool        ok = true;
    std::string errorMessage;
};

SP_API ShaderBlocks SplitShaderBlocks(std::string_view source);
SP_API ShaderBlocks ExpandShaderBlocks(const ShaderBlocks& blocks, const std::string& baseDirectory = {});
