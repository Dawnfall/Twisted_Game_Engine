#include "ShaderParser/ShaderParser.h"
#include "ShaderParser/ShaderData.h"
#include "ShaderParser/ShaderFile.h"
#include "ShaderParser/ShaderReflection.h"
#include "Utils.h"

#include "ShaderParser/Imports/MVP_Core.h"
#include "ShaderParser/Imports/Lights_Core.h"
#include "ShaderParser/Shaders/Default_Shader.h"
#include "ShaderParser/Shaders/Debug_Normal_Shader.h"
#include "ShaderParser/Shaders/Debug_UV_Shader.h"

#include <unordered_set>

static bool ParseImport(std::string_view line, std::string& outName)
{
    std::string_view trimmed = TrimLeft(line);
    if (!trimmed.starts_with("#import"))
        return false;

    std::string_view rest = trimmed.substr(7);
    size_t i = 0;
    while (i < rest.size() && (rest[i] == ' ' || rest[i] == '\t'))
        ++i;

    if (i >= rest.size() || rest[i] != '"')
        return false;
    ++i;

    size_t start = i;
    while (i < rest.size() && rest[i] != '"')
        ++i;

    if (i >= rest.size())
        return false;

    outName = std::string(rest.substr(start, i - start));
    return true;
}

static bool ParseInclude(std::string_view line, std::string& outPath)
{
    std::string_view trimmed = TrimLeft(line);
    if (!trimmed.starts_with("#include"))
        return false;

    std::string_view rest = trimmed.substr(8);
    size_t i = 0;
    while (i < rest.size() && (rest[i] == ' ' || rest[i] == '\t'))
        ++i;

    if (i >= rest.size() || rest[i] != '"')
        return false;
    ++i;

    size_t start = i;
    while (i < rest.size() && rest[i] != '"')
        ++i;

    if (i >= rest.size())
        return false;

    outPath = std::string(rest.substr(start, i - start));
    return true;
}

// -------------------------------------------------------
// Recursive preprocessor
// -------------------------------------------------------

static PreprocessorResult PreprocessImpl(
    std::string_view                                    source,
    const fs::path&                                     currentDir,
    std::unordered_set<std::string>&                    visitedFiles,
    std::unordered_set<std::string>&                    importStack,
    const std::unordered_map<std::string, std::string>& imports)
{
    PreprocessorResult result;

    size_t pos = 0;
    while (pos < source.size())
    {
        size_t eol        = source.find('\n', pos);
        bool   hasNewline = eol != std::string_view::npos;
        std::string_view line = source.substr(pos, hasNewline ? eol - pos : source.size() - pos);
        pos = hasNewline ? eol + 1 : source.size();

        std::string includePath;
        std::string importName;

        if (ParseInclude(line, includePath))
        {
            fs::path    fullPath  = (currentDir / includePath).lexically_normal();
            std::string canonical = fullPath.string();

            if (visitedFiles.contains(canonical))
                continue;

            if (!fs::exists(fullPath))
            {
                result.ok           = false;
                result.errorMessage = "Could not open include: " + canonical;
                return result;
            }

            visitedFiles.insert(canonical);

            auto nested = PreprocessImpl(ReadFile(fullPath), fullPath.parent_path(), visitedFiles, importStack, imports);
            if (!nested.ok)
                return nested;

            result.source += nested.source;
            if (!result.source.empty() && result.source.back() != '\n')
                result.source += '\n';
        }
        else if (ParseImport(line, importName))
        {
            auto it = imports.find(importName);
            if (it == imports.end())
            {
                result.ok           = false;
                result.errorMessage = "Unknown import: \"" + importName + "\"";
                return result;
            }

            if (importStack.contains(importName))
            {
                result.ok           = false;
                result.errorMessage = "Circular import detected: \"" + importName + "\"";
                return result;
            }

            importStack.insert(importName);
            auto nested = PreprocessImpl(it->second, currentDir, visitedFiles, importStack, imports);
            importStack.erase(importName);

            if (!nested.ok)
                return nested;

            result.source += nested.source;
            if (!result.source.empty() && result.source.back() != '\n')
                result.source += '\n';
        }
        else
        {
            result.source += line;
            if (hasNewline)
                result.source += '\n';
        }
    }

    return result;
}

// -------------------------------------------------------
// ShaderParser
// -------------------------------------------------------

ShaderParser::ShaderParser()
{
    RegisterImport("MVP_Core_Vert",    Twisted::MVP_Core_Vert);
    RegisterImport("MVP_Core_Frag",    Twisted::MVP_Core_Frag);
    RegisterImport("Lights_Core_Frag", Twisted::Lights_Core_Frag);

    ImportShaderFile(Twisted::DefaultShaderName,     Twisted::DefaultShader);
    ImportShaderFile(Twisted::DebugNormalShaderName, Twisted::DebugNormalShader);
    ImportShaderFile(Twisted::DebugUVShaderName,     Twisted::DebugUVShader);
}

ShaderParser& ShaderParser::Get()
{
    static ShaderParser s_instance;
    return s_instance;
}

void ShaderParser::RegisterImport(const std::string& name, const std::string& glsl)
{
    m_imports[name] = glsl;
}

void ShaderParser::ImportShaderFile(const std::string& name, const std::string& source)
{
    ShaderBlocks blocks = SplitShaderBlocks(source);
    if (!blocks.ok)
        return;

    // Expand #imports using this instance directly — avoids calling Get() during construction
    auto expand = [&](const std::optional<std::string>& src) -> std::optional<std::string>
    {
        if (!src) return std::nullopt;
        PreprocessorResult r = Preprocess(*src);
        return r.ok ? std::optional<std::string>(std::move(r.source)) : std::nullopt;
    };

    ShaderData data;
    data.vert = expand(blocks.vert);
    data.frag = expand(blocks.frag);
    data.geom = expand(blocks.geom);
    if (blocks.ref)
        data.reflection = ParseShaderRef(*blocks.ref);

    m_shaders[name] = std::move(data);
}

PreprocessorResult ShaderParser::Preprocess(std::string_view source, const std::string& baseDirectory) const
{
    fs::path baseDir = baseDirectory.empty() ? fs::current_path() : fs::path(baseDirectory);

    std::unordered_set<std::string> visitedFiles;
    std::unordered_set<std::string> importStack;
    return PreprocessImpl(source, baseDir, visitedFiles, importStack, m_imports);
}

const ShaderData* ShaderParser::GetShader(const std::string& name) const
{
    auto it = m_shaders.find(name);
    return it != m_shaders.end() ? &it->second : nullptr;
}
