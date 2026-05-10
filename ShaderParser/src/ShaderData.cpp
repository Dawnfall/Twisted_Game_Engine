#include "ShaderParser/ShaderData.h"
#include "ShaderParser/ShaderFile.h"
#include "ShaderParser/ShaderReflection.h"

ShaderData ProcessShaderFile(std::string_view source, const std::string& baseDirectory)
{
    ShaderData result;

    ShaderBlocks blocks = SplitShaderBlocks(source);
    if (!blocks.ok)
    {
        result.ok           = false;
        result.errorMessage = std::move(blocks.errorMessage);
        return result;
    }

    ShaderBlocks expanded = ExpandShaderBlocks(blocks, baseDirectory);
    if (!expanded.ok)
    {
        result.ok           = false;
        result.errorMessage = std::move(expanded.errorMessage);
        return result;
    }

    result.vert = std::move(expanded.vert);
    result.frag = std::move(expanded.frag);
    result.geom = std::move(expanded.geom);

    if (expanded.ref)
    {
        result.reflection = ParseShaderRef(*expanded.ref);
        if (!result.reflection.ok)
        {
            result.ok           = false;
            result.errorMessage = std::move(result.reflection.errorMessage);
        }
    }

    return result;
}
