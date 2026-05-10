#include "ShaderParser/ShaderFile.h"
#include "ShaderParser/ShaderParser.h"

static std::optional<std::string> ExtractBlock(
    std::string_view source,
    std::string_view openTag,
    std::string_view closeTag,
    bool&            ok,
    std::string&     error)
{
    size_t openPos = source.find(openTag);
    if (openPos == std::string_view::npos)
        return std::nullopt;

    size_t contentStart = openPos + openTag.size();
    if (contentStart < source.size() && source[contentStart] == '\r') ++contentStart;
    if (contentStart < source.size() && source[contentStart] == '\n') ++contentStart;

    size_t closePos = source.find(closeTag, contentStart);
    if (closePos == std::string_view::npos)
    {
        ok    = false;
        error = std::string("Missing closing tag: ") + std::string(closeTag);
        return std::nullopt;
    }

    size_t contentEnd = closePos;
    if (contentEnd > contentStart && source[contentEnd - 1] == '\n') --contentEnd;
    if (contentEnd > contentStart && source[contentEnd - 1] == '\r') --contentEnd;

    return std::string(source.substr(contentStart, contentEnd - contentStart));
}

ShaderBlocks ExpandShaderBlocks(const ShaderBlocks& blocks, const std::string& baseDirectory)
{
    if (!blocks.ok)
        return blocks;

    ShaderBlocks result;
    ShaderParser& parser = ShaderParser::Get();

    auto expand = [&](const std::optional<std::string>& src) -> std::optional<std::string>
    {
        if (!src)
            return std::nullopt;
        PreprocessorResult r = parser.Preprocess(*src, baseDirectory);
        if (!r.ok)
        {
            result.ok           = false;
            result.errorMessage = std::move(r.errorMessage);
        }
        return r.ok ? std::optional<std::string>(std::move(r.source)) : std::nullopt;
    };

    result.vert = expand(blocks.vert); if (!result.ok) return result;
    result.frag = expand(blocks.frag); if (!result.ok) return result;
    result.geom = expand(blocks.geom); if (!result.ok) return result;
    result.ref  = blocks.ref; // not GLSL — pass through as-is
    return result;
}

ShaderBlocks SplitShaderBlocks(std::string_view source)
{
    ShaderBlocks result;

    result.vert = ExtractBlock(source, "###VERT###", "###VERT/###", result.ok, result.errorMessage);
    if (!result.ok) return result;

    result.frag = ExtractBlock(source, "###FRAG###", "###FRAG/###", result.ok, result.errorMessage);
    if (!result.ok) return result;

    result.geom = ExtractBlock(source, "###GEOM###", "###GEOM/###", result.ok, result.errorMessage);
    if (!result.ok) return result;

    result.ref  = ExtractBlock(source, "###REF###",  "###REF/###",  result.ok, result.errorMessage);

    return result;
}
