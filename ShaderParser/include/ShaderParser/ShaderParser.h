#pragma once
#include "API.h"
#include "ShaderData.h"

#include <string>
#include <string_view>
#include <unordered_map>

struct SP_API PreprocessorResult
{
    std::string source;
    bool        ok = true;
    std::string errorMessage;
};

class SP_API ShaderParser
{
public:
    static ShaderParser& Get();

    void RegisterImport(const std::string& name, const std::string& glsl);
    void ImportShaderFile(const std::string& name, const std::string& source);

    PreprocessorResult  Preprocess(std::string_view source, const std::string& baseDirectory = {}) const;
    const ShaderData*   GetShader(const std::string& name) const;

    ShaderParser(const ShaderParser&)            = delete;
    ShaderParser& operator=(const ShaderParser&) = delete;

private:
    ShaderParser();

    std::unordered_map<std::string, std::string> m_imports;
    std::unordered_map<std::string, ShaderData>  m_shaders;
};
